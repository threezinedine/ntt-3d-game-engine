#pragma once
#include "allocator.h"
#include "pch.h"

namespace ntt {

/**
 * The allocator which can be used as a stack, user can use checkpoints to roll back to previous states.
 */
class StackAllocator : public Allocator
{
public:
	/**
	 * Only a fixed size stack allocator is supported for now.
	 *
	 * @param totalSize The total size of the stack allocator in bytes.
	 */
	StackAllocator(Size totalSize);
	~StackAllocator() override;

public:
	StackAllocator(const StackAllocator&)			 = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;
	StackAllocator(StackAllocator&&)				 = delete;
	StackAllocator& operator=(StackAllocator&&)		 = delete;

public:
	/**
	 * Be used for active obtaining the current checkpoint of the stack allocator.
	 */
	inline void* getCurrentCheckpoint() const
	{
		return m_pCurrentAddress;
	}

	/**
	 * The main interface of the stack allocator, which will roll back to the certain
	 * 		address for renewing the stack.
	 *
	 * @param pCheckpoint The checkpoint address to roll back to.
	 */
	void resetToCheckpoint(void* pCheckpoint);

protected:
	virtual void* allocateImpl(Size size, Size alignment = 0) override;
	virtual void  deallocateImpl(void* ptr) override;

protected:
	inline virtual AllocatorType getAllocatorType() const override
	{
		return ALLOCATOR_TYPE_STACK;
	}

private:
	Size  m_size;			 // Total size of the stack allocator in bytes
	void* m_pStart;			 // The beginning of the stack bank.
	void* m_pCurrentAddress; // The next allocation address will be placed here.
};

} // namespace ntt