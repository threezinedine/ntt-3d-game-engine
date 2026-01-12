#if NTT_USE_GRAPHICS_VULKAN & NTT_USE_GLFW

#include "graphics/vulkan/surface_vulkan.h"

#if NTT_USE_GLFW
#include <GLFW/glfw3.h>
#endif

namespace ntt {

#if NTT_USE_GLFW
Surface::Surface(GLFWwindow* pWindow)
	: m_pWindow(pWindow)
{
	glfwGetWindowSize(pWindow, &m_width, &m_height);
}
#else
Surface::Surface()
{
}
#endif

void Surface::CreateVkSurface()
{
	NTT_ASSERT(Renderer::GetVkInstance() != VK_NULL_HANDLE);

#if NTT_USE_GLFW
	VK_ASSERT(glfwCreateWindowSurface(Renderer::GetVkInstance(), m_pWindow, nullptr, &m_vkSurface));
#endif
	detectFeatures();
}

void Surface::DestroyVkSurface()
{
	vkDestroySurfaceKHR(Renderer::GetVkInstance(), m_vkSurface, nullptr);
}

Surface::~Surface()
{
}

void Surface::Bind()
{
#if NTT_USE_GLFW
	glfwMakeContextCurrent(m_pWindow);
#endif
}

static VkSurfaceFormatKHR chooseFormat(const Array<VkSurfaceFormatKHR>& formats);
static VkPresentModeKHR	  choosePresentMode(const Array<VkPresentModeKHR>& presentModes);

void Surface::detectFeatures()
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Renderer::GetVkPhysicalDevice(), m_vkSurface, &capabilities);

	m_currentTransform = capabilities.currentTransform;

	m_minImageCount = 3;
	if (m_minImageCount > capabilities.maxImageCount && capabilities.maxImageCount != 0)
	{
		m_minImageCount = capabilities.maxImageCount;
	}
	else
	{
		m_minImageCount = capabilities.minImageCount + 1;
	}

	u32 surfaceFormatsCount = 0u;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Renderer::GetVkPhysicalDevice(), m_vkSurface, &surfaceFormatsCount, nullptr);
	NTT_ASSERT(surfaceFormatsCount > 0);
	Array<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(
		Renderer::GetVkPhysicalDevice(), m_vkSurface, &surfaceFormatsCount, surfaceFormats.data());

	m_surfaceFormat = chooseFormat(surfaceFormats);

	u32 surfacePresentModesCount = 0u;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		Renderer::GetVkPhysicalDevice(), m_vkSurface, &surfacePresentModesCount, nullptr);
	NTT_ASSERT(surfacePresentModesCount > 0);
	Array<VkPresentModeKHR> surfacePresentModes(surfacePresentModesCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		Renderer::GetVkPhysicalDevice(), m_vkSurface, &surfacePresentModesCount, surfacePresentModes.data());

	m_presentMode = choosePresentMode(surfacePresentModes);
}

static VkSurfaceFormatKHR chooseFormat(const Array<VkSurfaceFormatKHR>& formats)
{
	for (auto format : formats)
	{
		if (format.format == VK_FORMAT_R8G8B8A8_SRGB)
		{
			return format;
		}
	}

	return formats[0];
}

static VkPresentModeKHR choosePresentMode(const Array<VkPresentModeKHR>& presentModes)
{
	NTT_UNUSED(presentModes);

	return VK_PRESENT_MODE_FIFO_KHR;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN & NTT_USE_GLFW
