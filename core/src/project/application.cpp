#include "project/application.h"
#include "graphics/graphics.h"
#include "input/input.h"

namespace ntt {

Application::Application(const String& projectFilePath)
	: m_isInitialized(NTT_FALSE)
{
	m_pProject = CreateScope<Project>();
}

Application::~Application()
{
}

void Application::Start()
{
	NTT_ASSERT(!m_isInitialized);

	NTT_APPLICATION_LOG_INFO("Starting application ...");

	startBeginImpl();
	Input::Initialize();

	startEndImpl();

	NTT_APPLICATION_LOG_INFO("Application started.");

	m_isInitialized = NTT_TRUE;
}

void Application::Update(f32 deltaTime)
{
	NTT_ASSERT(m_isInitialized);

	updateBeginImpl(deltaTime);

	Renderer::BeginFrame();

	Renderer::EndFrame();
	Renderer::PresentFrame();

	updateEndImpl(deltaTime);
}

void Application::Shutdown()
{
	NTT_ASSERT(m_isInitialized);

	NTT_APPLICATION_LOG_INFO("Shutting down application ...");

	shutdownBeginImpl();

	Input::Shutdown();
	shutdownEndImpl();

	NTT_APPLICATION_LOG_INFO("Application shut down.");
}

} // namespace ntt
