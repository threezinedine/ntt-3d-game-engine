#pragma once

#include "allocator.h"
#include "pch.h"

namespace ntt {

/**
 * The allocator which can be reset to the original state, freeing all allocations at once.
 */
class LinearAllocator : public Allocator
{
public:
	/**
	 * Only a fixed size linear allocator is supported for now.
	 *
	 * @param totalSize The total size of the linear allocator in bytes.
	 */
	LinearAllocator(Size totalSize);
	~LinearAllocator() override;

public:
	LinearAllocator(const LinearAllocator&)			   = delete;
	LinearAllocator& operator=(const LinearAllocator&) = delete;
	LinearAllocator(LinearAllocator&&)				   = delete;
	LinearAllocator& operator=(LinearAllocator&&)	   = delete;

public:
	/**
	 * Once called and clear all allocations made so far.
	 */
	void reset();

protected:
	virtual void* allocateImpl(Size size, Size alignment = 0) override;
	virtual void  deallocateImpl(void* ptr) override;

	inline virtual AllocatorType getAllocatorType() const override
	{
		return ALLOCATOR_TYPE_LINEAR;
	}

private:
	Size  m_size;			 // Total size of the linear allocator in bytes
	void* m_pStart;			 // The beginning of the linear bank.
	void* m_pCurrentAddress; // The next allocation address will be placed here.
};

} // namespace ntt
