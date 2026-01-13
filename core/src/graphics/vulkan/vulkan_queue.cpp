#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_queue.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

GraphicQueue::GraphicQueue(Reference<Device> pDevice, VkQueue vkQueue)
	: m_pDevice(pDevice)
	, m_vkQueue(vkQueue)
{
}

GraphicQueue::GraphicQueue(const GraphicQueue& other)
	: m_pDevice(other.m_pDevice)
	, m_vkQueue(other.m_vkQueue)
{
}

GraphicQueue::GraphicQueue(GraphicQueue&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_vkQueue(other.m_vkQueue)
{
	other.m_pDevice = nullptr;
	other.m_vkQueue = VK_NULL_HANDLE;
}

GraphicQueue::~GraphicQueue()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN