#if NTT_IDE_APPLICATION

#include "project/ide_application.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "platforms/graphics/graphics.h"

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

namespace ntt {

IDEApplication::IDEApplication()
	: Application()
{
}

IDEApplication::~IDEApplication()
{
}

void IDEApplication::startBeginImpl()
{
	InitializeWindowingSystem();
	m_pWindow = CreateScope<Window>("NTT IDE Window", 1280, 720);
	m_pWindow->Initialize();
}

void IDEApplication::startEndImpl()
{
	Renderer::AttachSurface(m_pWindow->GetSurface());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking

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
}

void IDEApplication::updateBeginImpl(f32 deltaTime)
{
	m_pWindow->PollEvents();

	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		m_pWindow->Close();
	}

#if NTT_USE_GRAPHICS_OPENGL
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui new frame not implemented yet."
#else
#error "No graphics API defined for ImGui new frame."
#endif // NTT_USE_GRAPHICS_OPENGL

	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Hello, IDE!");
	ImGui::Text("This is the NTT IDE Application using ImGui!");
	ImGui::End();

	ImGui::ShowDemoWindow();
}

void IDEApplication::updateEndImpl(f32 deltaTime)
{
	ImGui::Render();

	IVec2 windowSize = m_pWindow->Size();

#if NTT_USE_GRAPHICS_OPENGL
#if NTT_USE_GLFW
	glViewport(0, 0, windowSize.x, windowSize.y);
#endif // NTT_USE_GLFW

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif NTT_USE_GRAPHICS_VULKAN
#error "Vulkan ImGui new frame not implemented yet."
#else
#error "No graphics API defined for ImGui new frame."
#endif // NTT_USE_GRAPHICS_OPENGL
}

void IDEApplication::shutdownBeginImpl()
{
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
}

void IDEApplication::shutdownEndImpl()
{
	m_pWindow->Shutdown();
	ShutdownWindowingSystem();
}

b8 IDEApplication::IsOpen() const
{
	return m_pWindow->IsOpen();
}

} // namespace ntt

#endif // NTT_IDE_APPLICATION