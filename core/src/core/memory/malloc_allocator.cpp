#include "core/memory/malloc_allocator.h"
#include <cstdlib>

namespace ntt {

void* MallocAllocator::allocateImpl(Size size, Size alignment)
{
	NTT_UNUSED(alignment);

	return malloc(size);
}

void MallocAllocator::deallocateImpl(void* ptr)
{
	free(ptr);
}

} // namespace ntt
