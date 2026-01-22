#pragma once

#include "pch.h"

namespace ntt {

/**
 * All possible algorithm types for memory allocators which are
 * 	implemented in the engine.
 */
enum AllocatorType : u8
{
	ALLOCATOR_TYPE_MALLOC, ///< Allocator using standard malloc and free
	ALLOCATOR_TYPE_LINEAR, ///< Linear allocator which can be reset
	ALLOCATOR_TYPE_STACK,  ///< Stack allocator with checkpoint support
	ALLOCATOR_TYPE_COUNT,
};

/**
 * Convert AllocatorType enum to string representation.
 *
 * @param type The AllocatorType enum value.
 * @return The string representation of the AllocatorType.
 */
const char* AllocatorTypeToString(AllocatorType type);

} // namespace ntt
