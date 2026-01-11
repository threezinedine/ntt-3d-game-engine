#pragma once
#include "project_impl.h"
#include "types.h"
#include <platforms/common.h>

namespace ntt {

/**
 * The global access point for the whole runtime application.
 * This is the interface for the user-defined application class.
 */
class Application
{
public:
	Application();
	virtual ~Application();

public:
	void OnStart();
	void OnUpdate(f32 deltaTime);
	void OnShutdown();

public:
	/**
	 * Default empty user-defined start implementation.
	 */
	virtual void onStartImpl() {};

	/**
	 * Default empty user-defined update implementation.
	 * @param deltaTime The time elapsed since the last frame.
	 */
	virtual void onUpdateImpl(f32 deltaTime) {};

	/**
	 * Default empty user-defined shutdown implementation.
	 */
	virtual void onShutdownImpl() {};

private:
	Scope<Project> m_pProject;
};

} // namespace ntt
