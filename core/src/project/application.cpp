#include "project/application.h"

namespace ntt {

Application::Application(const String& projectFilePath)
{
	m_pProject = CreateScope<Project>();
}

Application::~Application()
{
}

void Application::OnStart()
{
	onStartImpl();
}

void Application::OnUpdate(f32 deltaTime)
{
	onUpdateImpl(deltaTime);
}

void Application::OnShutdown()
{
	onShutdownImpl();
}

} // namespace ntt
