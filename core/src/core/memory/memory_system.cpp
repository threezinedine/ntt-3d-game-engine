#include "core/memory/memory_system.h"
#include "core/memory/malloc_allocator.h"
#include "memory_system_internal.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>

#if NTT_PLATFORM_LINUX
#include <cxxabi.h>
#include <execinfo.h>
#elif NTT_PLATFORM_WINDOWS
#error "Memory leak tracking is not implemented for Windows yet."
#else
#error "Memory leak tracking is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX

namespace ntt {

Allocator* MemorySystem::pDefaultAllocator = nullptr;

#if NTT_MEMORY_LEAK_TRACKING
#if NTT_MEMORY_LEAK_DEBUG
#define NTT_BACK_BUFFER_SIZE 1024

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

	pDefaultAllocator = new MallocAllocator();
}

static u32 extractAddress(char* pOutBuffer, const char* line);

void MemorySystem::Shutdown()
{
#if NTT_MEMORY_LEAK_TRACKING

	if (pMemoryBlockListHead != nullptr)
	{
		char	exePath[NTT_BACK_BUFFER_SIZE / 2] = {0};
		ssize_t len								  = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
		if (len == -1 || len >= (ssize_t)sizeof(exePath))
		{
			print("Failed to read /proc/self/exe", CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, true);
		}

		exePath[len] = '\0'; // null-terminate it

		char reportHeader[NTT_BACK_BUFFER_SIZE];
		snprintf(reportHeader,
				 NTT_BACK_BUFFER_SIZE,
				 "Memory Leak Detected: Total Leaked Memory = %zu bytes",
				 totalAllocatedMemory);
		print(reportHeader, CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT, true);

		print("Memory Leak Report:", CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT, true);
		char** symbols = backtrace_symbols(pMemoryBlockListHead->info.callStack,
										   static_cast<int>(pMemoryBlockListHead->info.callStackCount));

		// print memory leak report with call stacks
		for (u32 callStackDepth = 0u; callStackDepth < pMemoryBlockListHead->info.callStackCount; ++callStackDepth)
		{
			char addressBuffer[64];
			extractAddress(addressBuffer, symbols[callStackDepth]);

			char cmd[NTT_BACK_BUFFER_SIZE];
			snprintf(cmd, NTT_BACK_BUFFER_SIZE, "addr2line -e %s -f -C -p %s", exePath, addressBuffer);

			print(cmd, CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, false);

			FILE* pipe = popen(cmd, "r");
			if (!pipe)
			{
				print("Failed to open pipe for addr2line.", CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, true);
				continue;
			}
			char buffer[NTT_BACK_BUFFER_SIZE];
			fgets(buffer, NTT_BACK_BUFFER_SIZE, pipe);
			pclose(pipe);
			print(buffer, CONSOLE_COLOR_MAGENTA, CONSOLE_COLOR_DEFAULT, false);
		}
	}

#if NTT_MEMORY_LEAK_DEBUG
#else  // NTT_MEMORY_LEAK_DEBUG
	NTT_ASSERT_MSG(pMemoryBlockListHead == nullptr,
				   "Memory leak tracking is enabled, but memory leak debug mode is off. "
				   "Cannot report memory leaks without debug information.");
#endif // NTT_MEMORY_LEAK_DEBUG
#endif // NTT_MEMORY_LEAK_TRACKING

	delete pDefaultAllocator;
	pDefaultAllocator = nullptr;
}

static u32 extractAddress(char* pOutBuffer, const char* line)
{
	u32 lineCharCount = strlen(line);

	s32 startIndex = -1;
	s32 endIndex   = -1;

	for (u32 charIndex = 0u; charIndex < lineCharCount; ++charIndex)
	{
		if (line[charIndex] == '(')
		{
			startIndex = static_cast<s32>(charIndex) + 1;
		}
		else if (line[charIndex] == ')')
		{
			endIndex = static_cast<s32>(charIndex);
			break;
		}
	}

	NTT_ASSERT(startIndex != -1 && endIndex != -1 && endIndex > startIndex);
	u32 addressLength = static_cast<u32>(endIndex - startIndex);
	memcpy(pOutBuffer, &line[startIndex], addressLength);
	pOutBuffer[addressLength] = '\0';

	return addressLength;
}

#if NTT_DEBUG
#if NTT_PLATFORM_LINUX
void GetCallStack(void** outCallStack, u32& outCallStackCount, u32 maxDepth)
{
	backtrace(outCallStack, static_cast<int>(maxDepth));
	outCallStackCount = static_cast<u32>(backtrace(outCallStack, static_cast<int>(maxDepth)));
}
#elif NTT_PLATFORM_WINDOWS
#error "Memory leak tracking is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "Memory leak tracking is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX
#else  // NTT_DEBUG
void GetCallStack(void** outCallStack, u32& outCallStackCount, u32 maxDepth)
{
	NTT_UNUSED(outCallStack);
	NTT_UNUSED(outCallStackCount);
	NTT_UNUSED(maxDepth);
}
#endif // NTT_DEBUG

} // namespace ntt
