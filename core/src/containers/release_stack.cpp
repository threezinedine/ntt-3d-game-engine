#include "containers/release_stack.h"

namespace ntt {

ReleaseStack::ReleaseStack()
	: m_releaseEntries()
{
}

ReleaseStack::~ReleaseStack()
{
	if (!m_releaseEntries.empty())
	{
		ReleaseAll();
	}
}

void ReleaseStack::PushReleaseFunction(void* pUserData, Function<void(void*)> releaseFunction)
{
	ReleaseEntry entry;
	entry.p_userData	  = pUserData;
	entry.releaseFunction = releaseFunction;

	m_releaseEntries.push(entry);
}

void ReleaseStack::ReleaseAll(void)
{
	while (!m_releaseEntries.empty())
	{
		ReleaseEntry entry = m_releaseEntries.top();
		if (entry.releaseFunction)
		{
			entry.releaseFunction(entry.p_userData);
		}
		m_releaseEntries.pop();
	}
}

} // namespace ntt
