#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_fence.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Fence::Fence(Reference<Device> pDevice)
	: m_pDevice(pDevice)
{
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType				= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags				= VK_FENCE_CREATE_SIGNALED_BIT;

	VK_ASSERT(vkCreateFence(m_pDevice->GetVkDevice(), &fenceInfo, nullptr, &m_vkFence));
}
Fence::Fence(const Fence& other)
	: m_pDevice(other.m_pDevice)
	, m_vkFence(other.m_vkFence)
{
	// Note: Copying a fence does not create a new fence, both instances will refer to the same VkFence.
}
Fence::Fence(Fence&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_vkFence(other.m_vkFence)

{
	other.m_pDevice = nullptr;
	other.m_vkFence = VK_NULL_HANDLE;
}

void Fence::Wait(u64 timeout)
{
	VK_ASSERT(vkWaitForFences(m_pDevice->GetVkDevice(), 1, &m_vkFence, VK_FALSE, timeout));
}

void Fence::Reset()
{
	VK_ASSERT(vkResetFences(m_pDevice->GetVkDevice(), 1, &m_vkFence));
}

Fence::~Fence()
{
	if (m_vkFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(m_pDevice->GetVkDevice(), m_vkFence, nullptr);
	}
}

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN