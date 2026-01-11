#pragma once
#include "project_impl.h"
#include "types.h"
#include <platforms/common.h>

namespace ntt {

/**
 * The global access point for the whole runtime application.
 * This is the interface for the user-defined application class.
 */
class NTT_BINDING Application
{
public:
	/**
	 * Constructor and Destructor
	 * @param projectFilePath The file path to the project configuration file.
	 * Relative path to the executable.
	 */
	Application(const String& projectFilePath);
	virtual ~Application();

public:
	/**
	 * Be called at the starting of the runtime.
	 */
	void OnStart() NTT_BINDING;

	/**
	 * Be called every frame to update the application.
	 * @param deltaTime The time elapsed since the last frame.
	 */
	void OnUpdate(f32 deltaTime) NTT_BINDING;

	/**
	 * Be called at the shutting down of the runtime.
	 */
	void OnShutdown() NTT_BINDING;

protected:
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
