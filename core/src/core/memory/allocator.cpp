#include "core/memory/memory.h"
#include "memory_system_internal.h"

namespace ntt {

Allocator::~Allocator()
{
}

void* Allocator::allocate(Size size, Size alignment)
{
#if NTT_MEMORY_LEAK_TRACKING
	void* ptr = allocateImpl(size, alignment);

#if NTT_MEMORY_LEAK_DEBUG
	// Create a new memory block info
	MemoryBlockNode* newNode = new MemoryBlockNode();

	MemoryBlockInfo& blockInfo = newNode->info;
	blockInfo.pAddress		   = ptr;
	blockInfo.size			   = size;
	blockInfo.allocatorType	   = getAllocatorType();
	blockInfo.pBacktraceInfo   = getBacktrace();

	// Insert the new node at the head of the memory block list
	newNode->pNext		 = pMemoryBlockListHead;
	pMemoryBlockListHead = newNode;
#endif // NTT_MEMORY_LEAK_DEBUG

	totalAllocatedMemory += size;

	return ptr;
#else  // NTT_MEMORY_LEAK_TRACKING
	return allocateImpl(size, alignment);
#endif // NTT_MEMORY_LEAK_TRACKING
}

void Allocator::deallocate(void* ptr, Size size)
{
#if NTT_MEMORY_LEAK_TRACKING

#if NTT_MEMORY_LEAK_DEBUG
	MemoryBlockNode* prevNode	 = nullptr;
	MemoryBlockNode* currentNode = pMemoryBlockListHead;

	// Search for the memory block in the linked list
	while (currentNode != nullptr)
	{
		if (currentNode->info.pAddress == ptr)
		{
			if (currentNode->info.size != size)
			{
				print("Warning: Deallocating memory block with size mismatch.",
					  CONSOLE_COLOR_YELLOW,
					  CONSOLE_COLOR_DEFAULT,
					  true);
			}

			// Found the block, remove it from the list
			if (prevNode != nullptr)
			{
				prevNode->pNext = currentNode->pNext;
			}
			else
			{
				pMemoryBlockListHead = currentNode->pNext;
			}

			deleteBacktrace(currentNode->info.pBacktraceInfo);
			delete currentNode;

			break;
		}

		prevNode	= currentNode;
		currentNode = currentNode->pNext;
	}

#endif // NTT_MEMORY_LEAK_DEBUG

	totalAllocatedMemory -= size;

#endif // NTT_MEMORY_LEAK_TRACKING

	deallocateImpl(ptr);
}

const char* AllocatorTypeToString(AllocatorType type)
{
	switch (type)
	{
	case ALLOCATOR_TYPE_MALLOC:
		return "Malloc Allocator";
	case ALLOCATOR_TYPE_LINEAR:
		return "Linear Allocator";
	case ALLOCATOR_TYPE_STACK:
		return "Stack Allocator";
	case ALLOCATOR_TYPE_COUNT:
		NTT_UNREACHABLE();
		return "Unknown Allocator Type";
	}

} // namespace ntt