#if NTT_USE_GRAPHICS_VULKAN
#pragma once

#include "platforms/common.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {

class GraphicQueue;

class Device
{
public:
	Device(VkPhysicalDevice vkPhysicalDevice, const Array<const char*>& extensions, const Array<const char*>& layers);
	NTT_DELETE_COPY(Device)
	NTT_DELETE_MOVE(Device)
	~Device();

public:
	inline b8 IsInitialized()
	{
		return m_vkDevice != VK_NULL_HANDLE;
	}

	inline VkPhysicalDevice GetVkPhysicalDevice() const
	{
		return m_vkPhysicalDevice;
	}

	inline VkDevice& GetVkDevice()
	{
		return m_vkDevice;
	}

private:
	VkPhysicalDevice	m_vkPhysicalDevice;
	VkDevice			m_vkDevice;
	Scope<GraphicQueue> m_renderQueue;
	Scope<GraphicQueue> m_transferQueue;
	Scope<GraphicQueue> m_computeQueue;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN