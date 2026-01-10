#if NTT_USE_GRAPHICS_OPENGL
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "graphics/renderer.h"

namespace ntt {
b8 Renderer::m_isInitialized = NTT_FALSE;

void Renderer::Initialize()
{
	NTT_ASSERT(!m_isInitialized);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_isInitialized = NTT_TRUE;
}

void Renderer::AttachSurface(Ref<Surface>& surface)
{
	NTT_ASSERT(m_isInitialized);
	surface->Bind();
	NTT_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

void Renderer::Shutdown()
{
	NTT_ASSERT(m_isInitialized);
	m_isInitialized = NTT_FALSE;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL