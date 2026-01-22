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
	ALLOCATOR_TYPE_COUNT,
};

} // namespace ntt
