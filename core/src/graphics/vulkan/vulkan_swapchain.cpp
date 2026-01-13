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
	m_fences.resize(m_imagesCount);
	m_imageReadySemaphores.resize(m_imagesCount);
	m_renderFinisedSemaphores.resize(m_imagesCount);
	for (u32 imageIndex = 0u; imageIndex < m_imagesCount; ++imageIndex)
	{
		m_images.emplace_back(pDevice,
							  images[imageIndex],
							  getFormatFromVkFormat(pSurface->GetSurfaceFormat().format),
							  ImageType::IMAGE_TYPE_2D);

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType				= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags				= VK_FENCE_CREATE_SIGNALED_BIT;
		VK_ASSERT(vkCreateFence(pDevice->GetVkDevice(), &fenceInfo, nullptr, &m_fences[imageIndex]));

		m_releaseStack.PushReleaseFunction(&m_fences[imageIndex], [&](void* pUserData) {
			vkDestroyFence(m_pDevice->GetVkDevice(), *(VkFence*)pUserData, nullptr);
		});

		VkSemaphoreCreateInfo imageReadyInfo = {};
		imageReadyInfo.sType				 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VK_ASSERT(
			vkCreateSemaphore(pDevice->GetVkDevice(), &imageReadyInfo, nullptr, &m_imageReadySemaphores[imageIndex]));

		m_releaseStack.PushReleaseFunction(&m_imageReadySemaphores[imageIndex], [&](void* pUserData) {
			vkDestroySemaphore(m_pDevice->GetVkDevice(), *(VkSemaphore*)pUserData, nullptr);
		});

		VkSemaphoreCreateInfo renderFinishedInfo = {};
		renderFinishedInfo.sType				 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VK_ASSERT(vkCreateSemaphore(
			pDevice->GetVkDevice(), &renderFinishedInfo, nullptr, &m_renderFinisedSemaphores[imageIndex]));

		m_releaseStack.PushReleaseFunction(&m_renderFinisedSemaphores[imageIndex], [&](void* pUserData) {
			vkDestroySemaphore(m_pDevice->GetVkDevice(), *(VkSemaphore*)pUserData, nullptr);
		});
	}
}

Swapchain::~Swapchain()
{
	m_releaseStack.ReleaseAll();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN