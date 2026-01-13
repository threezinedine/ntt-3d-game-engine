#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_command_pool.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

CommandPool::CommandPool(Device* pDevice, u32 familyIndex)
	: m_pDevice(pDevice)
	, m_familyIndex(familyIndex)
	, m_releaseStack()
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType					 = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex		 = familyIndex;

	VK_ASSERT(vkCreateCommandPool(pDevice->GetVkDevice(), &poolInfo, nullptr, &m_vkPool));

	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyCommandPool(m_pDevice->GetVkDevice(), m_vkPool, nullptr); });
}

CommandPool::~CommandPool()
{
	m_releaseStack.ReleaseAll();
}

Array<CommandBuffer> CommandPool::AllocateCommandBuffers(u32 count)
{
	Array<VkCommandBuffer> vkBuffers(count);

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType						 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool				 = m_vkPool;
	allocateInfo.level						 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount			 = count;

	VK_ASSERT(vkAllocateCommandBuffers(m_pDevice->GetVkDevice(), &allocateInfo, vkBuffers.data()));

	Array<CommandBuffer> buffers;
	buffers.reserve(count);
	for (const auto& vkBuffer : vkBuffers)
	{
		buffers.emplace_back(m_pDevice, this, vkBuffer);
	}

	return buffers;
}

void CommandPool::FreeCommandBuffers(u32 count, CommandBuffer* buffers)
{
	Array<VkCommandBuffer> vkBuffers(count);
	for (u32 bufferIndex = 0u; bufferIndex < count; ++bufferIndex)
	{
		vkBuffers[bufferIndex] = buffers[bufferIndex].GetVkCommandBuffer();
	}

	vkFreeCommandBuffers(m_pDevice->GetVkDevice(), m_vkPool, count, vkBuffers.data());
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN
