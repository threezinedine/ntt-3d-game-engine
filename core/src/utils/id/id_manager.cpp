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

ID IDManager::RegisterID(IDTypes type, IDUniqueType uniqueId)
{
	auto it = m_activeIDs.find(ID(0, type, uniqueId));
	if (it != m_activeIDs.end())
	{
		return *it;
	}

	ID newID(0, type, uniqueId);

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

void IDManager::UnRegisterID(const ID& id)
{
	LockID(id);

	m_activeIDs.erase(id);

	UnlockID(id);

	m_activeMutexes.erase(id);
}

ID* IDManager::GetGlobalID(const ID& id)
{
	auto it = m_activeIDs.find(id);

	if (it == m_activeIDs.end())
	{
		return NTT_NULLPTR;
	}

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
