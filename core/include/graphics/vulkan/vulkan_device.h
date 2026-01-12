#if NTT_USE_GRAPHICS_VULKAN
#pragma once

#include "platforms/common.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {

class Device
{
public:
	Device();
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

	void Create(VkPhysicalDevice physicalDevice);

	void Destroy();

private:
	VkPhysicalDevice m_vkPhysicalDevice;
	VkDevice		 m_vkDevice;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN