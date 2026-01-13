#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_swapchain.h"
#include "graphics/image.h"
#include "graphics/surface.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Swapchain::Swapchain(Device* pDevice, Reference<Surface> pSurface)
	: m_pDevice(pDevice)
	, m_pSurface(pSurface)
{
	VkExtent2D extent = {};
	extent.width	  = pSurface->GetSize().x;
	extent.height	  = pSurface->GetSize().y;

	u32 renderQueueFamily = Renderer::GetRenderQueueFamilyIndex();

	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType					   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface				   = pSurface->GetVkSurface();
	swapchainInfo.imageColorSpace		   = pSurface->GetSurfaceFormat().colorSpace;
	swapchainInfo.imageFormat			   = pSurface->GetSurfaceFormat().format;
	swapchainInfo.presentMode			   = pSurface->GetPresentMode();
	swapchainInfo.minImageCount			   = pSurface->GetMinImageCount();
	swapchainInfo.compositeAlpha		   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // swapchain no need alpha
	swapchainInfo.clipped				   = VK_TRUE;
	swapchainInfo.imageSharingMode		   = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.imageUsage			   = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.queueFamilyIndexCount	   = 1;
	swapchainInfo.pQueueFamilyIndices	   = &renderQueueFamily;
	swapchainInfo.imageExtent			   = extent;
	swapchainInfo.preTransform			   = pSurface->GetCurrentTransform();
	swapchainInfo.imageArrayLayers		   = 1;

	VK_ASSERT(vkCreateSwapchainKHR(pDevice->GetVkDevice(), &swapchainInfo, nullptr, &m_vkSwapchain));
	NTT_RENDERER_LOG_DEBUG("Swapchain created.");
	m_releaseStack.PushReleaseFunction(NTT_NULLPTR, [&](void*) {
		vkDestroySwapchainKHR(m_pDevice->GetVkDevice(), m_vkSwapchain, nullptr);
		NTT_RENDERER_LOG_DEBUG("Swapchain destroyed.");
	});

	// Acquire images
	vkGetSwapchainImagesKHR(pDevice->GetVkDevice(), m_vkSwapchain, &m_imagesCount, nullptr);
	Array<VkImage> images(m_imagesCount);
	vkGetSwapchainImagesKHR(pDevice->GetVkDevice(), m_vkSwapchain, &m_imagesCount, images.data());

	m_images.reserve(m_imagesCount);

	for (u32 imageIndex = 0u; imageIndex < m_imagesCount; ++imageIndex)
	{
		m_images.emplace_back(pDevice,
							  images[imageIndex],
							  getFormatFromVkFormat(pSurface->GetSurfaceFormat().format),
							  ImageType::IMAGE_TYPE_2D);
	}
}

Swapchain::~Swapchain()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN