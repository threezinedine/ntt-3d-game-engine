#pragma once

#include "core/memory/allocator.h"
#include "core/memory/memory_system.h"
#include "core/memory/memory_utils.h"
#include "pch.h"

namespace ntt {

#define NTT_ARRAY_INITIAL_CAPACITY 4

class Allocator;

/**
 * The dynamic vector class which can hold elements of type T. If the array append exceeds
 * the current capacity, it will automatically resize to accommodate more elements.
 */
template <typename T>
class Array
{
public:
	Array(Allocator* pArrayAllocator = nullptr)
		: Array(NTT_ARRAY_INITIAL_CAPACITY, pArrayAllocator)
	{
	}

	Array(u32 capacity, Allocator* pArrayAllocator = nullptr)
		: m_pAllocator(pArrayAllocator)
	{
		NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

		m_count	   = 0;
		m_capacity = capacity;
		m_pData	   = (T*)pAllocator->allocate(m_capacity * sizeof(T));
	}

	/**
	 * Deep copy of the array
	 */
	Array(const Array& other)
		: m_pAllocator(other.m_pAllocator)
	{
		NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

		m_count	   = other.m_count;
		m_capacity = other.m_capacity;
		m_pData	   = (T*)pAllocator->allocate(m_capacity * sizeof(T));
		MemoryCopy(m_pData, other.m_pData, m_count * sizeof(T));
	}

	Array(Array&& other) noexcept
		: m_pAllocator(other.m_pAllocator)
		, m_pData(other.m_pData)
		, m_count(other.m_count)
		, m_capacity(other.m_capacity)
	{
		other.m_pData	 = nullptr;
		other.m_count	 = 0;
		other.m_capacity = 0;
	}

	~Array()
	{
		if (m_pData != nullptr)
		{
			NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);
			pAllocator->deallocate(m_pData, m_capacity * sizeof(T));
			m_pData = nullptr;
		}
	}

public:
	/**
	 * Get the current number of elements in the array.
	 *
	 * @return The number of elements currently stored in the array.
	 */
	inline u32 count() const
	{
		return m_count;
	}

	inline T& operator[](Size index)
	{
		if (index >= m_count)
		{
			NTT_RAISE_ERROR(EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS, "Array index out of bounds");
		}
		return m_pData[index];
	}

	inline const T& operator[](Size index) const
	{
		if (index >= m_count)
		{
			NTT_RAISE_ERROR(EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS, "Array index out of bounds");
		}
		return m_pData[index];
	}

	void append(const T& element)
	{
		if (m_count >= m_capacity)
		{
			resize(m_capacity == 0 ? NTT_ARRAY_INITIAL_CAPACITY : m_capacity * 2);
		}

		new (&m_pData[m_count++]) T(element);
	}

	void append(T&& element)
	{
		if (m_count >= m_capacity)
		{
			resize(m_capacity == 0 ? NTT_ARRAY_INITIAL_CAPACITY : m_capacity * 2);
		}

		new (&m_pData[m_count++]) T((T&&)element);
	}

	void clear()
	{
		for (u32 index = 0u; index < m_count; ++index)
		{
			m_pData[index].~T();
		}
		m_count = 0;
	}

	/**
	 * Magic method to change the current capacity of the array. All the existing elements
	 * will be preserved up to the new size or the old size, whichever is smaller.
	 *
	 * @param newCapacity The new capacity for the array.
	 */
	void resize(Size newCapacity)
	{
		if (newCapacity <= m_capacity)
		{
			NTT_RAISE_ERROR(EXCEPTION_TYPE_INVALID_ARGUMENT, "Resizing to a smaller size is not supported yet.");
		}

		NTT_USE_ALLOCATOR(pAllocator, m_pAllocator);

		T* newData = (T*)pAllocator->allocate(newCapacity * sizeof(T));
		MemoryCopy(newData, m_pData, m_count * sizeof(T));
		pAllocator->deallocate(m_pData, m_capacity * sizeof(T));
		m_pData	   = newData;
		m_capacity = newCapacity;
	}

private:
	Allocator* m_pAllocator; // The allocator used for memory management, can be nullptr, then use default allocator
	T*		   m_pData;		 // Pointer to the data buffer
	Size	   m_count;		 // Current number of elements
	Size	   m_capacity;	 // Actual allocated capacity
};

} // namespace ntt
