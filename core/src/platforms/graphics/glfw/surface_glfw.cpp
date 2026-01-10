#if NTT_USE_GLFW

#include "platforms/graphics/surface/surface_glfw.h"
#include <GLFW/glfw3.h>

namespace ntt {

Surface::Surface(GLFWwindow* window)
	: m_window(window)
{
}

Surface::~Surface()
{
}

void Surface::Bind()
{
	glfwMakeContextCurrent(m_window);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL
