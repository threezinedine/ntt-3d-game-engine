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

	glewExperimental = GL_TRUE;
	NTT_ASSERT(glewInit() == GLEW_OK);

	auto version = glGetString(GL_VERSION);
	NTT_RENDERER_LOG_INFO("OpenGL Version: %s", version);

	try
	{
		glCreateShader(GL_VERTEX_SHADER);
	} catch (...)
	{
		NTT_RENDERER_LOG_FATAL("Failed to create OpenGL shader. GLEW might not be initialized correctly.");
		NTT_UNREACHABLE();
	}
}

void Renderer::BeginFrame()
{
	NTT_ASSERT(m_isInitialized);
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