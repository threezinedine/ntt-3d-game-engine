#pragma once
#include "pch.h"

/**
 * @file memory_utils.h
 * @brief This file is reserved for future memory utility functions and classes.
 */

namespace ntt {

/**
 * Copy a block of memory from source to destination.
 *
 * @param destination Pointer to the destination memory block.
 * @param source Pointer to the source memory block.
 * @param size The number of bytes to copy.
 */
void MemoryCopy(void* destination, const void* source, Size size);

/**
 * Set a block of memory to a specific value.
 *
 * @param destination Pointer to the memory block to set.
 * @param value The value to set each byte to.
 * @param size The number of bytes to set.
 */
void MemorySet(void* destination, u8 value, Size size);

} // namespace ntt
