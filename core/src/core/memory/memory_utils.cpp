#include "core/memory/memory_utils.h"
#include <cstring>

namespace ntt {

void MemoryCopy(void* destination, const void* source, Size size)
{
	memcpy(destination, source, size);
}

void MemorySet(void* destination, u8 value, Size size)
{
	memset(destination, value, size);
}

} // namespace ntt
