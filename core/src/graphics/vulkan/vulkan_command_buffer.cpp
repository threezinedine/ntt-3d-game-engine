#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_command_pool.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

CommandBuffer::CommandBuffer(Device* pDevice, CommandPool* pPool, VkCommandBuffer defaultBuffer)
	: m_pDevice(pDevice)
	, m_pPool(pPool)
{
	if (defaultBuffer != VK_NULL_HANDLE)
	{
		m_vkCommandBuffer = defaultBuffer;
	}
	else
	{
		VkCommandBufferAllocateInfo bufferInfo = {};
		bufferInfo.sType					   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		bufferInfo.commandPool				   = pPool->GetVkCommandPool();
		bufferInfo.commandBufferCount		   = 1;
		bufferInfo.level					   = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VK_ASSERT(vkAllocateCommandBuffers(pDevice->GetVkDevice(), &bufferInfo, &m_vkCommandBuffer));
	}
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

void CommandBuffer::StartRecord()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType					   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	vkBeginCommandBuffer(m_vkCommandBuffer, &beginInfo);
}

void CommandBuffer::EndRecord()
{
	vkEndCommandBuffer(m_vkCommandBuffer);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN