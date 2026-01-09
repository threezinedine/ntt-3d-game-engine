#include "platforms/trap.h"

#if NTT_PLATFORM_LINUX
#include <csignal>
#endif

namespace ntt {

#if NTT_COMPILER_MSVC
void debugBreak()
{
	__debugbreak();
}
#else
void debugBreak()
{
	raise(SIGTRAP);
}
#endif

} // namespace ntt
