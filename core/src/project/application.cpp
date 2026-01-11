#include "project/application.h"
#include "graphics/graphics.h"
#include "input/input.h"

namespace ntt {

Application::Application(const String& projectFilePath)
{
	m_pProject = CreateScope<Project>();
}

Application::~Application()
{
}

void Application::Start()
{
	NTT_APPLICATION_LOG_INFO("Starting application ...");

	startBeginImpl();
	Input::Initialize();

	startEndImpl();

	NTT_APPLICATION_LOG_INFO("Application started.");
}

void Application::Update(f32 deltaTime)
{
	updateBeginImpl(deltaTime);

	Renderer::BeginFrame();

	Renderer::EndFrame();
	Renderer::PresentFrame();

	updateEndImpl(deltaTime);
}

void Application::Shutdown()
{
	NTT_APPLICATION_LOG_INFO("Shutting down application ...");

	shutdownBeginImpl();

	Input::Shutdown();
	shutdownEndImpl();

	NTT_APPLICATION_LOG_INFO("Application shut down.");
}

} // namespace ntt
