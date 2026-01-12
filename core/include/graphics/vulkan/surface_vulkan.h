#if NTT_USE_GRAPHICS_VULKAN

#pragma once
#include "platforms/common.h"
#include "vulkan_common.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace ntt {

class Surface
{
public:
#if NTT_USE_GLFW
	Surface(GLFWwindow* pWindow);
#else
	Surface();
#endif
	NTT_DELETE_COPY(Surface)
	NTT_DELETE_MOVE(Surface)
	~Surface();

	void CreateVkSurface();
	void DestroyVkSurface();

#if NTT_USE_GLFW
	inline GLFWwindow* GetGLFWWindow() const
	{
		return m_pWindow;
	}
#endif

	inline VkSurfaceKHR GetVkSurface() const
	{
		return m_vkSurface;
	}

	void Bind();

public:
	inline u32 GetMinImageCount() const
	{
		return m_minImageCount;
	}

	inline VkSurfaceFormatKHR GetSurfaceFormat() const
	{
		return m_surfaceFormat;
	}

	inline VkPresentModeKHR GetPresentMode() const
	{
		return m_presentMode;
	}

	inline IVec2 GetSize()
	{
		return IVec2{m_width, m_height};
	}

	inline VkSurfaceTransformFlagBitsKHR GetCurrentTransform()
	{
		return m_currentTransform;
	}

private:
	void detectFeatures();

private:
#if NTT_USE_GLFW
	GLFWwindow* m_pWindow;
#endif
	VkSurfaceKHR m_vkSurface;

private:
	i32 m_width;
	i32 m_height;

private:
	u32							  m_minImageCount;
	VkSurfaceFormatKHR			  m_surfaceFormat;
	VkSurfaceTransformFlagBitsKHR m_currentTransform;
	VkPresentModeKHR			  m_presentMode;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN && NTT_USE_GLFW