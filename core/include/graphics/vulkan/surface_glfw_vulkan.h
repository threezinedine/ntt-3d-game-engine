#if NTT_USE_GRAPHICS_VULKAN && NTT_USE_GLFW

#pragma once
#include "platforms/common.h"
#include "vulkan_common.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ntt {

class Surface
{
public:
	Surface(GLFWwindow* window);
	NTT_DELETE_COPY(Surface)
	NTT_DELETE_MOVE(Surface)
	~Surface();

	void CreateVkSurface();

	inline GLFWwindow* GetGLFWWindow() const
	{
		return m_pWindow;
	}

	inline VkSurfaceKHR GetVkSurface() const
	{
		return m_vkSurface;
	}

	void Bind();

private:
	GLFWwindow*	 m_pWindow;
	VkSurfaceKHR m_vkSurface;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN && NTT_USE_GLFW
