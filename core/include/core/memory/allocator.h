#pragma once
#include "allocator_types.h"
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
	void* allocate(Size size, Size alignment = 0);

	/**
	 * Deallocate a previously allocated memory block.
	 *
	 * @param ptr A pointer to the memory block to be freed.
	 * @param size The size of the memory block to deallocate, in bytes.
	 */
	void deallocate(void* ptr, Size size);

protected:
	/**
	 * Allocate implementation to be provided by derived classes.
	 */
	virtual void* allocateImpl(Size size, Size alignment) = 0;

	/**
	 * Deallocate implementation to be provided by derived classes.
	 */
	virtual void deallocateImpl(void* ptr) = 0;

protected:
	/**
	 * Get the type of the allocator.
	 *
	 * @return The AllocatorType enum value representing the type of allocator.
	 */
	virtual AllocatorType getAllocatorType() const = 0;
};

} // namespace ntt
