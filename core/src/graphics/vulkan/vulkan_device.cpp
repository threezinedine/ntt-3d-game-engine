#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Device::Device()
	: m_vkPhysicalDevice(VK_NULL_HANDLE)
	, m_vkDevice(VK_NULL_HANDLE)
	, m_releaseStack()
{
}

Device::~Device()
{
}

void Device::Create(VkPhysicalDevice physicalDevice)
{
	NTT_ASSERT(physicalDevice == VK_NULL_HANDLE);
	NTT_ASSERT(m_vkDevice == VK_NULL_HANDLE);
}

void Device::Destroy()
{
	m_releaseStack.ReleaseAll();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN