#include "core/memory/stack_allocator.h"
#include <cstdlib>

namespace ntt {

StackAllocator::StackAllocator(Size totalSize)
	: Allocator()
	, m_size(totalSize)
{
	m_pStart		  = malloc(totalSize);
	m_pCurrentAddress = m_pStart;
}

StackAllocator::~StackAllocator()
{
	free(m_pStart);
	m_pStart		  = nullptr;
	m_pCurrentAddress = nullptr;
}

void* StackAllocator::allocateImpl(Size size, Size alignment)
{
	NTT_UNUSED(alignment);
	NTT_ASSERT_MSG(static_cast<Size>(static_cast<u8*>(m_pCurrentAddress) - static_cast<u8*>(m_pStart) + size) <= m_size,
				   "StackAllocator out of memory.");

	void* result	  = m_pCurrentAddress;
	m_pCurrentAddress = static_cast<void*>(static_cast<u8*>(m_pCurrentAddress) + size);
	return result;
}

void StackAllocator::deallocateImpl(void* ptr)
{
	NTT_UNUSED(ptr);
}

void StackAllocator::resetToCheckpoint(void* pCheckpoint)
{
	NTT_ASSERT_MSG(pCheckpoint >= m_pStart && pCheckpoint <= static_cast<void*>(static_cast<u8*>(m_pStart) + m_size),
				   "Checkpoint is out of bounds of the stack allocator.");

	m_pCurrentAddress = pCheckpoint;
}

} // namespace ntt
