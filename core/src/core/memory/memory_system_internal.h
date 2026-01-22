#pragma once
#include "core/memory/allocator_types.h"
#include "pch.h"
#include "platforms/debug.h"

namespace ntt {

#if NTT_MEMORY_LEAK_TRACKING

#if NTT_MEMORY_LEAK_DEBUG
#define NTT_MEMORY_LEAK_STACK_DEPTH 32
#endif // NTT_MEMORY_LEAK_DEBUG

/**
 * Only be used when memory leak tracking is enabled and in memory leak debug mode.
 * This struct stores information about a single memory block allocation,
 * 	such as its size, allocation site, and call stack.
 */
struct MemoryBlockInfo
{
	void*		  pAddress;		 ///< The address of the allocated memory block
	Size		  size;			 ///< The size of the allocated memory block in bytes
	AllocatorType allocatorType; ///< The type of allocator used for this allocation

#if NTT_MEMORY_LEAK_DEBUG
	BacktraceInfo* pBacktraceInfo; ///< The call stack at the time of allocation

#endif // NTT_MEMORY_LEAK_DEBUG
};

/**
 * Only be used when memory leak tracking is enabled and in memory leak debug mode.
 * A node in the linked list storing all allocated memory blocks.
 */
struct MemoryBlockNode
{
	MemoryBlockInfo	 info;
	MemoryBlockNode* pNext;
};

/**
 * The head of the linked list storing all allocated memory blocks, only be used
 * when memory leak tracking is enabled and in memory leak debug mode
 */
extern MemoryBlockNode* pMemoryBlockListHead;

extern Size totalAllocatedMemory; ///< The total amount of memory currently allocated, in bytes

#endif // NTT_MEMORY_LEAK_TRACKING

} // namespace ntt
