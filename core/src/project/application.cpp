#include "project/application.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "platforms/platforms.h"
#include <filesystem>
#include <fstream>

#if NTT_USE_IMGUI
#include "imgui.h"

#if NTT_USE_GLFW
#include "imgui_impl_glfw.h"
#endif // NTT_USE_GLFW

#if NTT_USE_GRAPHICS_OPENGL
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#endif // NTT_USE_GRAPHICS_OPENGL

#if NTT_USE_GRAPHICS_VULKAN
#include "imgui_impl_vulkan.h"
#endif // NTT_USE_GRAPHICS_VULKAN
#endif // NTT_USE_IMGUI

namespace ntt {

Application::Application()
	: m_pProject(nullptr)
	, m_isInitialized(NTT_FALSE)
{
}

Application::~Application()
{
}

b8 Application::IsOpen() const
{
	if (m_pWindow)
	{
		return m_pWindow->IsOpen();
	}
	return NTT_FALSE;
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

void Application::Close()
{
	NTT_ASSERT(m_pWindow != nullptr);
	m_pWindow->Close();
}

void Application::Start()
{
	NTT_ASSERT(!m_isInitialized);

	NTT_APPLICATION_LOG_INFO("Starting application ...");

	InitializeWindowingSystem();
	m_pWindow = CreateScope<Window>("NTT Application Window", 1280, 720);
	m_pWindow->Initialize();
	Renderer::Initialize();

	Renderer::AttachSurface(m_pWindow->GetSurface());

#if NTT_USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	  // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding			  = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	style.FontScaleDpi = 2.25f;

#if NTT_USE_GRAPHICS_OPENGL
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow->GetSurface()->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui initialization not implemented yet."
#else
#error "No graphics API defined for ImGui initialization."
#endif // NTT_USE_GRAPHICS_OPENGL
#endif // NTT_USE_IMGUI

	startBeginImpl();
	Input::Initialize();

	startEndImpl();

	NTT_APPLICATION_LOG_INFO("Application started.");

	m_isInitialized = NTT_TRUE;
}

void Application::Update(f32 deltaTime)
{
	NTT_ASSERT(m_isInitialized);
	m_pWindow->PollEvents();

	updateBeginImpl(deltaTime);

#if NTT_USE_IMGUI
#if NTT_USE_GRAPHICS_OPENGL
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui new frame not implemented yet."
#else
#error "No graphics API defined for ImGui new frame."
#endif // NTT_USE_GRAPHICS_OPENGL
#endif // NTT_USE_IMGUI

	Renderer::BeginFrame();

	updateImpl(deltaTime);

	Renderer::EndFrame();

	updateEndImpl(deltaTime);

#if NTT_USE_IMGUI
	ImGui::Render();

	IVec2 windowSize = m_pWindow->Size();

#if NTT_USE_GRAPHICS_OPENGL
#if NTT_USE_GLFW
	glViewport(0, 0, windowSize.x, windowSize.y);
#endif // NTT_USE_GLFW

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui new frame not implemented yet."
#else
#error "No graphics API defined for ImGui new frame."
#endif // NTT_USE_GRAPHICS_OPENGL

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		m_pWindow->GetSurface()->Bind();
	}
#endif // NTT_USE_IMGUI

	Renderer::PresentFrame();
}

void Application::Shutdown()
{
	NTT_ASSERT(m_isInitialized);

	NTT_APPLICATION_LOG_INFO("Shutting down application ...");

	shutdownBeginImpl();

	Input::Shutdown();
	shutdownEndImpl();

#if NTT_USE_IMGUI
#if NTT_USE_GRAPHICS_OPENGL
	ImGui_ImplOpenGL3_Shutdown();

#if NTT_USE_GLFW
	ImGui_ImplGlfw_Shutdown();
#endif // NTT_USE_GLFW
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui new frame not implemented yet."
#else
#error "No graphics API defined for ImGui new frame."
#endif // NTT_USE_GRAPHICS_OPENGL

	ImGui::DestroyContext();
#endif // NTT_USE_IMGUI

	m_pWindow->Shutdown();
	Renderer::Shutdown();
	ShutdownWindowingSystem();

	NTT_APPLICATION_LOG_INFO("Application shut down.");
}

} // namespace ntt
