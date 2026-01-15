#if NTT_USE_GRAPHICS_OPENGL
// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "graphics/surface.h"

namespace ntt {
b8				   Renderer::m_isInitialized = NTT_FALSE;
Reference<Surface> Renderer::s_pSurface		 = nullptr;

void Renderer::Initialize()
{
	NTT_ASSERT(!m_isInitialized);

	NTT_RENDERER_LOG_INFO("OpenGL Renderer initialized.");
	m_isInitialized = NTT_TRUE;
}

void Renderer::AttachSurface(Reference<Surface> pSurface)
{
	NTT_ASSERT(m_isInitialized);

#if NTT_ENGINE_EDITOR_BINDING
	NTT_UNUSED(pSurface);
#else
	s_pSurface = pSurface;
	s_pSurface->Bind();
	NTT_RENDERER_LOG_DEBUG("Surface attached to OpenGL Renderer.");
#endif

	NTT_ASSERT(glewInit() == GLEW_OK);

	auto version = glGetString(GL_VERSION);
	NTT_RENDERER_LOG_INFO("OpenGL Version: %s", version);

	Scope<Shader> shader =
		CreateScope<Shader>(STRINGIFY(NTT_ENGINE_DIRECTORY) "core/assets/shaders/opengl/default.vert");

	shader->Compile();
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

#if !NTT_ENGINE_EDITOR_BINDING
	glfwSwapBuffers(s_pSurface->GetGLFWWindow());
#endif
}

void Renderer::Shutdown()
{
	NTT_ASSERT(m_isInitialized);
	NTT_RENDERER_LOG_INFO("OpenGL Renderer shut down.");
	m_isInitialized = NTT_FALSE;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL