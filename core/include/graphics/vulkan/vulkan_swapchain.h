#if NTT_USE_GRAPHICS_VULKAN

#pragma once
#include "containers/release_stack.h"
#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {

class Device;
class Image;
class Semaphore;

class Swapchain
{
public:
	Swapchain(Reference<Device> pDevice, Reference<Surface> pSurface);
	NTT_DELETE_COPY(Swapchain)
	NTT_DELETE_MOVE(Swapchain)
	~Swapchain();

public:
	inline VkSwapchainKHR& GetVkSwapchain()
	{
		return m_vkSwapchain;
	}

	inline u32 GetImageCounts() const
	{
		return m_imagesCount;
	}

	inline u32 GetCurrentImageIndex() const
	{
		return m_currentImageIndex;
	}

public:
	u32 AcquireNextImage(Semaphore& signalSemaphore);

private:
	VkSwapchainKHR	   m_vkSwapchain;
	Reference<Device>  m_pDevice;
	Reference<Surface> m_pSurface;
	ReleaseStack	   m_releaseStack;

private:
	u32			 m_imagesCount;
	Array<Image> m_images;

private:
	u32 m_currentImageIndex;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN