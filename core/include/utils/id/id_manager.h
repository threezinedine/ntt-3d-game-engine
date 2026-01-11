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
	 * @param uniqueId The unique identifier for the ID.
	 * @return A new unique ID of the specified type if not existed, if existed, return the existing latest ID (latest
	 * version).
	 *
	 * @note If a new ID is created, it can reuse a previously freed ID to optimize resource usage.
	 */
	ID RegisterID(IDTypes type, IDUniqueType uniqueId);

	/**
	 * Frees an ID, making it available for reuse.
	 *
	 * @param id The ID to free.
	 */
	void UnRegisterID(const ID& id);

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
};

} // namespace ntt
