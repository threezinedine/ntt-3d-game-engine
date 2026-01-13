#if NTT_USE_GRAPHICS_VULKAN

#pragma once
#include "containers/release_stack.h"
#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {

class Device;
class Image;

class Swapchain
{
public:
	Swapchain(Device* pDevice, Reference<Surface> pSurface);
	NTT_DELETE_COPY(Swapchain)
	NTT_DELETE_MOVE(Swapchain)
	~Swapchain();

public:
	inline VkSwapchainKHR& GetVkSwapchain()
	{
		return m_vkSwapchain;
	}

private:
	VkSwapchainKHR	   m_vkSwapchain;
	Device*			   m_pDevice;
	Reference<Surface> m_pSurface;
	ReleaseStack	   m_releaseStack;

private:
	u32				   m_imagesCount;
	Array<Image>	   m_images;
	Array<VkFence>	   m_fences;
	Array<VkSemaphore> m_imageReadySemaphores;
	Array<VkSemaphore> m_renderFinisedSemaphores;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN