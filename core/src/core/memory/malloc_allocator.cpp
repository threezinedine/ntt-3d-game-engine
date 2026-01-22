#include "core/memory/malloc_allocator.h"
#include <cstdlib>

namespace ntt {

void* MallocAllocator::allocate(Size size, Size alignment)
{
	NTT_UNUSED(alignment);

	return malloc(size);
}

void MallocAllocator::deallocate(void* ptr)
{
	free(ptr);
}

} // namespace ntt
