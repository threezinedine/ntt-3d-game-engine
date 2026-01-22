#include "core/memory/memory_system.h"
#include "core/memory/malloc_allocator.h"
#include "core/memory/stack_allocator.h"
#include "memory_system_internal.h"
#include <cstdio>

namespace ntt {

Allocator*		 MemorySystem::pDefaultAllocator = nullptr;
StackAllocator*	 MemorySystem::pStackAllocator	 = nullptr;
MallocAllocator* MemorySystem::pMallocAllocator	 = nullptr;

#if NTT_MEMORY_LEAK_TRACKING
#if NTT_MEMORY_LEAK_DEBUG

#define NTT_REPORT_HEADER_BUFFER_SIZE 256

MemoryBlockNode* pMemoryBlockListHead = nullptr;
#endif // NTT_MEMORY_LEAK_DEBUG

Size totalAllocatedMemory = 0;
#endif // NTT_MEMORY_LEAK_TRACKING

void MemorySystem::Initialize()
{
#if NTT_MEMORY_LEAK_TRACKING
#if NTT_MEMORY_LEAK_DEBUG
	pMemoryBlockListHead = nullptr;
#endif // NTT_MEMORY_LEAK_DEBUG
	totalAllocatedMemory = 0;
#endif // NTT_MEMORY_LEAK_TRACKING

	pStackAllocator	  = new StackAllocator(100 * NTT_MB); // 100 MB stack allocator
	pMallocAllocator  = new MallocAllocator();
	pDefaultAllocator = pMallocAllocator;
}

void MemorySystem::Shutdown()
{
#if NTT_MEMORY_LEAK_TRACKING

#if NTT_MEMORY_LEAK_DEBUG
	if (pMemoryBlockListHead != nullptr)
	{
		char reportHeader[NTT_REPORT_HEADER_BUFFER_SIZE];
		snprintf(reportHeader,
				 NTT_REPORT_HEADER_BUFFER_SIZE,
				 "Memory Leak Detected: Total Leaked Memory = %zu bytes",
				 totalAllocatedMemory);
		print(reportHeader, CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT, true);

		print("Memory Leak Report:", CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT, true);

		printBacktrace(pMemoryBlockListHead->info.pBacktraceInfo);
	}

#else  // NTT_MEMORY_LEAK_DEBUG
	NTT_ASSERT_MSG(pMemoryBlockListHead == nullptr,
				   "Memory leak tracking is enabled, but memory leak debug mode is off. "
				   "Cannot report memory leaks without debug information.");
#endif // NTT_MEMORY_LEAK_DEBUG
#endif // NTT_MEMORY_LEAK_TRACKING

	delete pStackAllocator;
	pStackAllocator = nullptr;

	delete pMallocAllocator;
	pMallocAllocator = nullptr;

	pDefaultAllocator = nullptr;
}

} // namespace ntt
