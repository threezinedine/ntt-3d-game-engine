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
	MallocAllocator(const MallocAllocator&)			   = delete;
	MallocAllocator& operator=(const MallocAllocator&) = delete;
	MallocAllocator(MallocAllocator&&)				   = delete;
	MallocAllocator& operator=(MallocAllocator&&)	   = delete;

protected:
	virtual void* allocateImpl(Size size, Size alignment = 0) override;
	virtual void  deallocateImpl(void* ptr) override;

public:
	inline virtual AllocatorType getAllocatorType() const override
	{
		return ALLOCATOR_TYPE_MALLOC;
	}
};

} // namespace ntt
