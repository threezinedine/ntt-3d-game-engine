#if NTT_USE_GRAPHICS_OPENGL & NTT_USE_GLFW

#include "graphics/opengl/surface_opengl.h"
#include <GLFW/glfw3.h>

namespace ntt {

Surface::Surface(GLFWwindow* pWindow)
	: m_pWindow(pWindow)
{
}

Surface::~Surface()
{
}

void Surface::Bind()
{
	glfwMakeContextCurrent(m_pWindow);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL & NTT_USE_GLFW
