#include "core/memory/memory_system.h"
#include "core/memory/malloc_allocator.h"

namespace ntt {

Allocator* MemorySystem::pDefaultAllocator = nullptr;

void MemorySystem::Initialize()
{
	pDefaultAllocator = new MallocAllocator();
}

void MemorySystem::Shutdown()
{
	delete pDefaultAllocator;
	pDefaultAllocator = nullptr;
}

} // namespace ntt
