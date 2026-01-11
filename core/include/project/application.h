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

public:
	void onStartImpl();

private:
	Scope<Project> m_pProject;
};

} // namespace ntt
