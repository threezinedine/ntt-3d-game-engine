#pragma once
#include "allocator_types.h"
#include "pch.h"

namespace ntt {

class Allocator;
class StackAllocator;
class MallocAllocator;
class LinearAllocator;

/**
 * Only be used when memory leak tracking is enabled.
 * Retrieve the current call stack up to the specified maximum depth.
 * In release builds, this function does nothing.
 *
 * @param outCallStack An array to store the call stack addresses.
 * @param outCallStackCount The number of frames retrieved in the call stack.
 * @param maxDepth The maximum depth of the call stack to retrieve.
 */
void GetCallStack(void** outCallStack, u32& outCallStackCount, u32 maxDepth);

/**
 * Stored the needed information about the memory usages, the global allocators, etc.
 * In the debug builds, it also tracks memory leaks and allocations.
 */
class MemorySystem
{
public:
	/**
	 * Starting the memory system, this method must be called as the first thing in the
	 *      engine initialization for both debug and release builds.
	 */
	static void Initialize();

	/**
	 * Release all the resources used by the memory system.
	 * This method must be called at the very end of the engine shutdown process.
	 * In debug builds, this will also report any memory leaks detected.
	 */
	static void Shutdown();

public:
	static inline Allocator* getDefaultAllocator()
	{
		return pDefaultAllocator;
	}

	static inline StackAllocator* getStackAllocator()
	{
		return pStackAllocator;
	}

	static inline MallocAllocator* getMallocAllocator()
	{
		return pMallocAllocator;
	}

	static inline LinearAllocator* getLinearAllocator()
	{
		return pLinearAllocator;
	}

	static inline void setDefaultAllocator(Allocator* pAllocator)
	{
		pDefaultAllocator = pAllocator;
	}

private:
	static Allocator*		pDefaultAllocator; ///< The allocate algorithm which is mostly used throughout the engine
	static StackAllocator*	pStackAllocator;   ///< The global stack allocator for temporary allocations
	static MallocAllocator* pMallocAllocator;  ///< The global malloc allocator
	static LinearAllocator* pLinearAllocator;  ///< The global linear allocator
};

#define NTT_USE_ALLOCATOR(dest, ptr)                                                                                   \
	Allocator* dest = ptr;                                                                                             \
	if (dest == nullptr)                                                                                               \
	{                                                                                                                  \
		dest = MemorySystem::getDefaultAllocator();                                                                    \
	}

} // namespace ntt
