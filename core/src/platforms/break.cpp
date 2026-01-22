#include "platforms/break.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <csignal>
#endif

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

} // namespace ntt
