#pragma once

#include "allocator.h"
#include "pch.h"

namespace ntt {

/**
 * A memory allocator which uses the standard C library malloc and free functions.
 */
class MallocAllocator : public Allocator
{
public:
	MallocAllocator()			= default;
	~MallocAllocator() override = default;

public:
	virtual void* allocate(Size size, Size alignment = 0) override;
	virtual void  deallocate(void* ptr) override;
};

} // namespace ntt
