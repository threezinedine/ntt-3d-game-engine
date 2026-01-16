#include "project/application.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include <filesystem>
#include <fstream>

namespace ntt {

Application::Application()
	: m_pProject(nullptr)
	, m_isInitialized(NTT_FALSE)
{
}

Application::~Application()
{
}

void Application::LoadProject(const String& projectFilePath)
{
	NTT_APPLICATION_LOG_INFO("Loading project from file path: %s", projectFilePath.c_str());

	// check if the file exists
	if (!std::filesystem::exists(projectFilePath))
	{
		NTT_APPLICATION_LOG_WARN("Project file does not exist: %s", projectFilePath.c_str());
		return;
	}

	// Unload current project if any
	if (m_pProject)
	{
		NTT_APPLICATION_LOG_INFO("Unloading current project ...");
		m_pProject.reset();
	}

	// Load new project
	std::ifstream fileStream(projectFilePath);
	if (!fileStream.is_open())
	{
		NTT_APPLICATION_LOG_ERROR("Failed to open project file: %s", projectFilePath.c_str());
		return;
	}

	ProjectDescription description = ProjectDescriptionFromJsonString(
		String(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()));
	m_pProject = CreateScope<Project>(description);

	NTT_APPLICATION_LOG_INFO("Project '%s' loaded successfully.", description.name.c_str());
}

void Application::Start()
{
	NTT_ASSERT(!m_isInitialized);

	NTT_APPLICATION_LOG_INFO("Starting application ...");

	startBeginImpl();
	Renderer::Initialize();
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
	Renderer::Shutdown();
	shutdownEndImpl();

	NTT_APPLICATION_LOG_INFO("Application shut down.");
}

} // namespace ntt
