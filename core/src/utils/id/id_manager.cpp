#include "utils/id/id_manager.h"
#include "utils/logger/logger.h"
#include <algorithm>

namespace ntt {

NTT_SINGLETON_DEFINE(IDManager)

IDManager::IDManager()
{
}

IDManager::~IDManager()
{
}

ID IDManager::CreateID(IDTypes type)
{
	ID newID;

	// Check if there are any freed IDs available for reuse
	if (!m_freedIDs.empty())
	{
		auto it = m_freedIDs.begin();
		newID	= *it;

		m_freedIDs.erase(it);
		newID.ResetVersion();
	}
	else
	{
		// Generate a new unique ID
		IDUniqueType newUniqueID = static_cast<IDUniqueType>(m_activeIDs.size() + 1);
		newID					 = ID(0, type, newUniqueID);
	}

	m_activeIDs.insert(newID);
	m_activeMutexes.try_emplace(newID);
	return newID;
}

void IDManager::LockID(const ID& id)
{
	NTT_ASSERT(m_activeMutexes.find(id) != m_activeMutexes.end());
	m_activeMutexes[id].lock();
}

void IDManager::UnlockID(const ID& id)
{
	NTT_ASSERT(m_activeMutexes.find(id) != m_activeMutexes.end());
	m_activeMutexes[id].unlock();
}

void IDManager::FreeID(const ID& id)
{
	LockID(id);

	m_activeIDs.erase(id);
	m_freedIDs.insert(id);

	UnlockID(id);
}

IDVersionType IDManager::GetCurrentVersion(const ID& id) const
{
	auto it = m_activeIDs.find(id);
	NTT_ASSERT(it != m_activeIDs.end());
	return it->m_version;
}

ID* IDManager::GetGlobalID(const ID& id)
{
	auto it = m_activeIDs.find(id);
	NTT_ASSERT(it != m_activeIDs.end());

	return const_cast<ID*>(&(*it));
}

void IDManager::UpdateID(const ID& id)
{
	auto it = m_activeIDs.find(id);
	NTT_ASSERT(it != m_activeIDs.end());

	ID& managedID = const_cast<ID&>(*it);
	managedID.m_version++;
}

} // namespace ntt
