#if NTT_USE_GRAPHICS_VULKAN & NTT_USE_GLFW

#include "graphics/vulkan/surface_glfw_vulkan.h"
#include <GLFW/glfw3.h>

namespace ntt {

Surface::Surface(GLFWwindow* window)
	: m_pWindow(window)
{
}

void Surface::CreateVkSurface()
{
	NTT_ASSERT(Renderer::GetVkInstance() != VK_NULL_HANDLE);

	VK_ASSERT(glfwCreateWindowSurface(Renderer::GetVkInstance(), m_pWindow, nullptr, &m_vkSurface));
}

Surface::~Surface()
{
}

void Surface::Bind()
{
	glfwMakeContextCurrent(m_pWindow);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN & NTT_USE_GLFW
