#pragma once

namespace ntt {

/**
 * This module which holds and frees all singleton instances used in the engine.
 */
class SingletonManager
{
public:
	/**
	 * Starting, allocating all needed resources for this manager. Must be called before any singleton usage.
	 */
	static void Initialize();

	/**
	 * Shutdown, freeing all allocated resources for this manager. Must be called at the end of the application.
	 */
	static void Shutdown();
};

} // namespace ntt
