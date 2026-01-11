#pragma once
#include "id_impl.h"
#include "platforms/common.h"

namespace ntt {

/**
 * Stores the informations and provides some features to work with IDs throughout
 * the engine, this includes checking validity, type, version, etc. This is multi-threading
 * safe.
 */
class NTT_SINGLETON IDManager
{
	NTT_SINGLETON_DECLARE(IDManager)

public:
	/**
	 * Acquires a new unique ID of the specified type. It can reuse freed IDs if available.
	 *
	 * @param type The type of the ID to create.
	 * @return A new unique ID of the specified type.
	 */
	ID CreateID(IDTypes type);

	/**
	 * Frees an ID, making it available for reuse.
	 *
	 * @param id The ID to free.
	 */
	void FreeID(const ID& id);

	/**
	 * Check the latest version of the given ID.
	 * @param id The ID to check.
	 * @return The latest version of the ID.
	 */
	IDVersionType GetCurrentVersion(const ID& id) const;

	/**
	 * Lock an ID to prevent it from being updated.
	 * @param id The ID to lock.
	 */
	void LockID(const ID& id);

	/**
	 * Get global ID through the application
	 * @return Pointer to the global ID. If the ID is not found, reeturn nullptr.
	 */
	ID* GetGlobalID(const ID& id);

	/**
	 * Thread-safe method to update the version of an ID.
	 */
	void UpdateID(const ID& id);

	/**
	 * Unlock an ID to allow it to be updated.
	 * @param id The ID to unlock.
	 */
	void UnlockID(const ID& id);

private:
	Set<ID>		   m_activeIDs;
	Map<ID, Mutex> m_activeMutexes;
	Set<ID>		   m_freedIDs;
};

} // namespace ntt
