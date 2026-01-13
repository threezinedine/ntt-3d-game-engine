#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_semaphore.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Semaphore::Semaphore(Reference<Device> pDevice)
	: m_pDevice(pDevice)
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType					= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.flags					= 0;

	VK_ASSERT(vkCreateSemaphore(m_pDevice->GetVkDevice(), &semaphoreInfo, nullptr, &m_vkSemaphore));
}
Semaphore::Semaphore(const Semaphore& other)
	: m_pDevice(other.m_pDevice)
	, m_vkSemaphore(other.m_vkSemaphore)
{
	// Note: Copying a semaphore does not create a new semaphore, both instances will refer to the same VkSemaphore.
}
Semaphore::Semaphore(Semaphore&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_vkSemaphore(other.m_vkSemaphore)

{
	other.m_vkSemaphore = VK_NULL_HANDLE;
}

Semaphore::~Semaphore()
{
	if (m_vkSemaphore != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(m_pDevice->GetVkDevice(), m_vkSemaphore, nullptr);
	}
}

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN