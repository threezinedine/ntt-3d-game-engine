#pragma once
#include "pch.h"

namespace ntt {

/**
 * The blueprint for all memory allocators which are used in some parts of the engine.
 */
class Allocator
{
public:
	virtual ~Allocator();

public:
	/**
	 * Create a memory block with the given size and alignment.
	 *
	 * @param size The size of the memory block to allocate, in bytes.
	 * @param alignment The alignment requirement for the memory block.
	 *      If alignment is 0, default alignment is used. Value is in bytes.
	 * @return A pointer to the allocated memory block.
	 */
	virtual void* allocate(Size size, Size alignment = 0) = 0;

	/**
	 * Deallocate a previously allocated memory block.
	 *
	 * @param ptr A pointer to the memory block to be freed.
	 */
	virtual void deallocate(void* ptr) = 0;
};

} // namespace ntt
