#pragma once

#include "core/containers/string.h"
#include "core/memory/allocator.h"
#include "core/memory/memory_system.h"
#include "core/memory/memory_utils.h"
#include "pch.h"
#include <immintrin.h>

/**
 * @file flat_map.h
 *
 * The new type of map container that uses a flat array to store key-value pairs.
 */

namespace ntt {

#define NTT_DEFAULT_FLAT_GROUPS 8
#define NTT_SLOT_PER_GROUP		16
#define NTT_MAX_LOAD_FACTOR		0.75f

#define NTT_FLAT_MAP_MASK						  0x7F // Only choose the key from 0 - 127
#define NTT_FLAT_MAP_GET_GROUP_VALUE(hashValue)	  ((hashValue) >> 3)
#define NTT_FLAT_MAP_GET_CONTROL_VALUE(hashValue) ((hashValue) & NTT_FLAT_MAP_MASK)

typedef u64 HashType;
typedef u8	HashControlType;

constexpr HashControlType NTT_HASH_CONTROL_EMPTY   = 0xFF;
constexpr HashControlType NTT_HASH_CONTROL_DELETED = 0x80;

/**
 * The interface which the FlatMap uses for hashing keys. The memory layout:
 *
 * ==========================================================================================
 * | Control Word | Entry 0 | Entry 1 | ... | Entry N | Control Word | Entry 0 | ...
 *               <--- Group 0 --->       			  |						<--- Group 1 --->
 * ==========================================================================================
 */
template <typename K>
HashType hash(const K& key);

template <typename K, typename V>
class FlatMap
{
public:
	/**
	 * The information so that the map will store for each entry, In flat map, all data are
	 * stored in a contiguous array for better cache performance.
	 */
	struct HashEntry
	{
		K key;
		V value;
	};

	/**
	 * Store all the fingerprints for a group of slots.
	 */
	union ControlWord {
		HashControlType slots[NTT_SLOT_PER_GROUP];
	};

public:
	FlatMap(Allocator* pMapAllocator = nullptr)
		: FlatMap(NTT_DEFAULT_FLAT_GROUPS, pMapAllocator)
	{
	}

	FlatMap(u32 groupsCount, Allocator* pMapAllocator = nullptr)
		: m_pAllocator(pMapAllocator)
		, m_entries(nullptr)
		, m_groupCount(0)
	{
		rehash(groupsCount);
	}

	FlatMap(const FlatMap& other)
		: m_pAllocator(other.m_pAllocator)
	{
	}

	FlatMap(FlatMap&& other) noexcept
		: m_pAllocator(other.m_pAllocator)
	{
		other.m_pAllocator = nullptr;
	}

	~FlatMap()
	{
		if (m_entries)
		{
			NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);
			pAllocator->deallocate(m_entries,
								   m_groupCount * (NTT_SLOT_PER_GROUP * sizeof(HashEntry) + sizeof(ControlWord)));
			m_entries	 = nullptr;
			m_groupCount = 0;
		}
	}

public:
	/**
	 * Check if the map contains the given key.
	 */
	bool contains(const K& key) const
	{
		HashEntry* pEntry = getEntryByKey(key);

		return pEntry != nullptr;
	}

	void insert(const K& key, const V& value)
	{
		HashType		hashValue	= hash(key);
		HashType		groupIndex	= computIndex(hashValue);
		HashControlType controlByte = NTT_FLAT_MAP_GET_CONTROL_VALUE(hashValue);

		u32 currentGroupIndex = static_cast<u32>(groupIndex);

		while (true)
		{
			ControlWord* pControlWord = getControlWordPtr(currentGroupIndex);
			__m128i		 control	  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pControlWord));

			__m128i emptyPattern = _mm_set1_epi8(static_cast<char>(NTT_HASH_CONTROL_EMPTY));
			__m128i cmpResult	 = _mm_cmpeq_epi8(control, emptyPattern);
			s32		mask		 = _mm_movemask_epi8(cmpResult);

			s32 slotIndex = __builtin_ctz(mask);

			if (slotIndex < 0 || slotIndex >= NTT_SLOT_PER_GROUP)
			{
				continue;
			}

			HashEntry* pEntry = getEntry(currentGroupIndex, static_cast<u32>(slotIndex));
			pEntry->key		  = key;
			pEntry->value	  = value;

			pControlWord->slots[slotIndex] = static_cast<HashControlType>(controlByte);
			break;
		}
	}

private:
	HashType computIndex(HashType hashValue) const
	{
		return NTT_FLAT_MAP_GET_GROUP_VALUE(hashValue) % m_groupCount;
	}

	u32 getControlWordOffset(u32 groupIndex) const
	{
		return groupIndex * (NTT_SLOT_PER_GROUP * sizeof(HashEntry) + sizeof(ControlWord));
	}

	ControlWord* getControlWordPtr(u32 groupIndex) const
	{
		return reinterpret_cast<ControlWord*>(static_cast<u8*>(m_entries) + getControlWordOffset(groupIndex));
	}

	HashEntry* getEntry(u32 groupIndex, u32 slotIndex) const
	{
		u32 entryOffset = getControlWordOffset(groupIndex) + sizeof(ControlWord) + slotIndex * sizeof(HashEntry);
		return reinterpret_cast<HashEntry*>(static_cast<u8*>(m_entries) + entryOffset);
	}

	/**
	 * @note return nullptr of non existed.
	 */
	HashEntry* getEntryByKey(const K& key) const
	{
		HashType		hashValue	= hash(key);
		HashType		groupIndex	= computIndex(hashValue);
		HashControlType controlByte = NTT_FLAT_MAP_GET_CONTROL_VALUE(hashValue);

		u32 currentGroupIndex = static_cast<u32>(groupIndex);

		while (true)
		{
			ControlWord* pControlWord = getControlWordPtr(currentGroupIndex);
			__m128i		 control	  = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pControlWord));

			__m128i matchPattern = _mm_set1_epi8(static_cast<char>(controlByte));
			__m128i cmpResult	 = _mm_cmpeq_epi8(control, matchPattern);
			s32		mask		 = _mm_movemask_epi8(cmpResult);

			while (mask != 0)
			{
				s32		   slotIndex = __builtin_ctz(mask);
				HashEntry* pEntry	 = getEntry(currentGroupIndex, static_cast<u32>(slotIndex));

				if (pEntry->key == key)
				{
					return pEntry;
				}

				mask &= ~(1 << slotIndex);
			}

			return nullptr;
		}

		return nullptr;
	}

	/**
	 * Be reshape the internal storage to accommodate the new number of groups.
	 * It be trigger at the first time when the map is created and when the load factor
	 * exceeds a certain threshold.
	 */
	void rehash(u32 newGroupsCount)
	{
		NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

		u32	  newSize	 = newGroupsCount * (NTT_SLOT_PER_GROUP * sizeof(HashEntry) + sizeof(ControlWord));
		void* newEntries = pAllocator->allocate(newSize, alignof(HashEntry));

		// mark all control words as empty

		for (u32 groupIndex = 0u; groupIndex < newGroupsCount; ++groupIndex)
		{
			MemorySet(static_cast<u8*>(newEntries) + getControlWordOffset(groupIndex),
					  NTT_HASH_CONTROL_EMPTY,
					  sizeof(ControlWord));
		}

		if (m_entries)
		{
			// currently not implemented
		}
		else
		{
		}

		m_entries	 = newEntries;
		m_groupCount = newGroupsCount;
	}

private:
	Allocator* m_pAllocator; // The allocator used for memory management, can be nullptr, then use default allocator
	void*	   m_entries;	 // Where the data are stored
	u32		   m_groupCount; // Number of groups
};

} // namespace ntt
