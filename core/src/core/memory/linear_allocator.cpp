#include "core/memory/linear_allocator.h"
#include <cstdlib>

namespace ntt {

LinearAllocator::LinearAllocator(Size totalSize)
	: Allocator()
	, m_size(totalSize)
{
	m_pStart		  = malloc(totalSize);
	m_pCurrentAddress = m_pStart;
}

LinearAllocator::~LinearAllocator()
{
	free(m_pStart);
	m_pStart		  = nullptr;
	m_pCurrentAddress = nullptr;
}

void* LinearAllocator::allocateImpl(Size size, Size alignment)
{
	NTT_UNUSED(alignment);
	NTT_ASSERT_MSG(static_cast<u8*>(m_pStart) + size <= static_cast<u8*>(m_pStart) + m_size,
				   "LinearAllocator out of memory.");

	void* result	  = m_pCurrentAddress;
	m_pCurrentAddress = static_cast<void*>(static_cast<u8*>(m_pCurrentAddress) + size);
	return result;
}

void LinearAllocator::deallocateImpl(void* ptr)
{
	NTT_UNUSED(ptr);
}

void LinearAllocator::reset()
{
	m_pCurrentAddress = m_pStart;
}

} // namespace ntt
