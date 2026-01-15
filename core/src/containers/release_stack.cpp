#include "containers/release_stack.h"

namespace ntt {

ReleaseStack::ReleaseStack()
	: m_releaseEntries()
{
}

ReleaseStack::ReleaseStack(ReleaseStack&& other) noexcept
{
	m_releaseEntries.swap(other.m_releaseEntries);
	// m_releaseEntries	   = std::move(other.m_releaseEntries);
	// other.m_releaseEntries = Stack<ReleaseEntry>();
	NTT_ASSERT(other.m_releaseEntries.empty());
}

ReleaseStack::~ReleaseStack()
{
	if (!m_releaseEntries.empty())
	{
		ReleaseAll();
	}
}

void ReleaseStack::PushReleaseFunction(void* pUserData, std::function<void(void*)> releaseFunction)
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
