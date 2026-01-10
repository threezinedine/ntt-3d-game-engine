#if NTT_USE_GRAPHICS_OPENGL
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics/renderer.h"

namespace ntt {
b8			 Renderer::m_isInitialized = NTT_FALSE;
Ref<Surface> Renderer::s_pSurface	   = nullptr;

void Renderer::Initialize()
{
	NTT_ASSERT(!m_isInitialized);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_isInitialized = NTT_TRUE;
}

void Renderer::AttachSurface(Ref<Surface>& pSurface)
{
	NTT_ASSERT(m_isInitialized);
	s_pSurface = pSurface;
	s_pSurface->Bind();
	NTT_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

void Renderer::BeginFrame()
{
	NTT_ASSERT(m_isInitialized);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::EndFrame()
{
	NTT_ASSERT(m_isInitialized);
	// Any end of frame operations can be added here
}

void Renderer::PresentFrame()
{
	NTT_ASSERT(m_isInitialized);
	glfwSwapBuffers(s_pSurface->GetGLFWWindow());
}

void Renderer::Shutdown()
{
	NTT_ASSERT(m_isInitialized);
	m_isInitialized = NTT_FALSE;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL