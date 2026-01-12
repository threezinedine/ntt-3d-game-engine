#pragma once
#include "platforms/common.h"

namespace ntt {

class ReleaseStack
{
public:
	ReleaseStack();
	~ReleaseStack();
	NTT_DELETE_COPY(ReleaseStack)
	NTT_DELETE_MOVE(ReleaseStack)

public:
	/**
	 * Pushes a release function onto the stack.
	 * @param pUserData Pointer to user data to be passed to the release function.
	 * @param releaseFunction Function to be called for releasing resources.
	 */
	void PushReleaseFunction(void* pUserData, Function<void(void*)> releaseFunction);

	/**
	 * Destroy all resources by calling all release functions in LIFO order.
	 */
	void ReleaseAll(void);

private:
	struct ReleaseEntry
	{
		void*				  p_userData;
		Function<void(void*)> releaseFunction;
	};

	Stack<ReleaseEntry> m_releaseEntries;
};

} // namespace ntt
