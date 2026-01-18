#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_command_pool.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

CommandBuffer::CommandBuffer(Device* pDevice, CommandPool* pPool, VkCommandBuffer buffer)
	: m_pDevice(pDevice)
	, m_pPool(pPool)
	, m_vkCommandBuffer(buffer)
{
}

CommandBuffer::CommandBuffer(const CommandBuffer& other)
	: m_pDevice(other.m_pDevice)
	, m_pPool(other.m_pPool)
	, m_vkCommandBuffer(other.m_vkCommandBuffer)
{
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_pPool(other.m_pPool)
	, m_vkCommandBuffer(other.m_vkCommandBuffer)
{
	other.m_pDevice			= nullptr;
	other.m_vkCommandBuffer = VK_NULL_HANDLE;
	other.m_pPool			= nullptr;
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::Reset()
{
	vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

void CommandBuffer::StartRecord(b8 isOneTimeSubmit)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType					   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags =
		isOneTimeSubmit ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
}

void CommandBuffer::EndRecord()
{
	vkEndCommandBuffer(m_vkCommandBuffer);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN