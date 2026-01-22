#pragma once

#include "core/memory/allocator.h"
#include "core/memory/memory_system.h"
#include "core/memory/memory_utils.h"
#include "pch.h"

/**
 * @file flat_map.h
 *
 * The new type of map container that uses a flat array to store key-value pairs.
 */

namespace ntt {

template <typename K, typename V>
class FlatMap
{
public:
	FlatMap(Allocator* pMapAllocator = nullptr)
		: m_pAllocator(pMapAllocator)
	{
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
	}

private:
	Allocator* m_pAllocator; // The allocator used for memory management, can be nullptr, then use default allocator
};

} // namespace ntt
