#include "platforms/debug.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <csignal>
#endif

#if NTT_PLATFORM_LINUX
#include <cxxabi.h>
#include <execinfo.h>
#elif NTT_PLATFORM_WINDOWS
#error "Memory leak tracking is not implemented for Windows yet."
#else
#error "Memory leak tracking is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX

#define NTT_BACKTRACE_BUFFER_SIZE 1024
#define NTT_BACKTRACE_MAX_DEPTH	  32

namespace ntt {

void debugBreak()
{
#if defined(_MSC_VER)
	__debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
	__builtin_trap();
#else
	// Fallback: raise SIGTRAP signal
	raise(SIGTRAP);
#endif
}

#if NTT_DEBUG

#if NTT_PLATFORM_LINUX
struct BacktraceInfo
{
	void* pCallStack[NTT_BACKTRACE_MAX_DEPTH];
	u32	  callStackCount;
};
#elif NTT_PLATFORM_WINDOWS
#error "BacktraceInfo is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "BacktraceInfo is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX

#else // NTT_DEBUG

struct BacktraceInfo
{
	// Empty struct for release builds
};

#endif // NTT_DEBUG

BacktraceInfo* GetBacktrace()
{
#if NTT_DEBUG

#if NTT_PLATFORM_LINUX

	BacktraceInfo* pOutBacktraceInfo = new BacktraceInfo();

	backtrace(pOutBacktraceInfo->pCallStack, static_cast<int>(NTT_BACKTRACE_MAX_DEPTH));
	pOutBacktraceInfo->callStackCount =
		static_cast<u32>(backtrace(pOutBacktraceInfo->pCallStack, static_cast<int>(NTT_BACKTRACE_MAX_DEPTH)));

	return pOutBacktraceInfo;

#elif NTT_PLATFORM_WINDOWS
#error "GetBacktrace is not implemented for Windows yet."
#else // NTT_PLATFORM_LINUX
#error "GetBacktrace is not implemented for this platform yet."
#endif // NTT_PLATFORM_LINUX

#else  // NTT_DEBUG
	NTT_UNUSED(pOutBacktraceInfo);

	return nullptr;
#endif // NTT_DEBUG
}

#if NTT_DEBUG
static u32 extractAddress(char* pOutBuffer, const char* line);
#endif // NTT_DEBUG

void PrintBacktrace(BacktraceInfo* pBacktraceInfo)
{
#if NTT_DEBUG
	char	exePath[NTT_BACKTRACE_BUFFER_SIZE / 2] = {0};
	ssize_t len									   = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
	if (len == -1 || len >= (ssize_t)sizeof(exePath))
	{
		print("Failed to read /proc/self/exe", CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, true);
	}

	exePath[len]   = '\0'; // null-terminate it
	char** symbols = backtrace_symbols(pBacktraceInfo->pCallStack, static_cast<int>(pBacktraceInfo->callStackCount));

	// print memory leak report with call stacks
	for (u32 callStackDepth = 0u; callStackDepth < pBacktraceInfo->callStackCount; ++callStackDepth)
	{
		char addressBuffer[64];
		extractAddress(addressBuffer, symbols[callStackDepth]);

		char cmd[NTT_BACKTRACE_BUFFER_SIZE];
		snprintf(cmd, NTT_BACKTRACE_BUFFER_SIZE, "addr2line -e %s -f -C -p %s", exePath, addressBuffer);
		print(cmd, CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, false);

		FILE* pipe = popen(cmd, "r");
		if (!pipe)
		{
			print("Failed to open pipe for addr2line.", CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, true);
			continue;
		}
		char buffer[NTT_BACKTRACE_BUFFER_SIZE];
		fgets(buffer, NTT_BACKTRACE_BUFFER_SIZE, pipe);
		pclose(pipe);
		print(buffer, CONSOLE_COLOR_MAGENTA, CONSOLE_COLOR_DEFAULT, false);
#else  // NTT_DEBUG
	UNUSED(pBacktraceInfo);
#endif // NTT_DEBUG
	}
}

#if NTT_DEBUG
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
#endif // NTT_DEBUG

} // namespace ntt
