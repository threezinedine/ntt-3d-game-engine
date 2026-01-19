#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_buffer.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Buffer::Buffer(Device* pDevice, u32 size, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags)
	: m_pDevice(pDevice)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType			  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage			  = usage;
	bufferInfo.sharingMode		  = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.size				  = size;

	VK_ASSERT(vkCreateBuffer(m_pDevice->GetVkDevice(), &bufferInfo, nullptr, &m_vkBuffer));

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetVkDevice(), m_vkBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType				  = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize		  = memoryRequirements.size;
	allocateInfo.memoryTypeIndex	  = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, flags);

	VK_ASSERT(vkAllocateMemory(m_pDevice->GetVkDevice(), &allocateInfo, nullptr, &m_vkMemory));

	VkDeviceSize offset = 0;

	VK_ASSERT(vkBindBufferMemory(m_pDevice->GetVkDevice(), m_vkBuffer, m_vkMemory, offset));
}

Buffer::Buffer(Buffer&& other)
	: m_pDevice(other.m_pDevice)
	, m_vkBuffer(other.m_vkBuffer)
	, m_vkMemory(other.m_vkMemory)
{
	other.m_pDevice	 = nullptr;
	other.m_vkBuffer = VK_NULL_HANDLE;
	other.m_vkMemory = VK_NULL_HANDLE;
}

Buffer::~Buffer()
{
	if (m_vkMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_pDevice->GetVkDevice(), m_vkMemory, nullptr);
	}

	if (m_vkBuffer)
	{
		vkDestroyBuffer(m_pDevice->GetVkDevice(), m_vkBuffer, nullptr);
	}
}

i32 Buffer::GetMemoryTypeIndex(u32 typeBits, VkMemoryPropertyFlags flags)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(Renderer::GetVkPhysicalDevice(), &memoryProperties);

	for (u32 memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypeCount; ++memoryTypeIndex)
	{
		VkMemoryPropertyFlags supportFlags = memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;
		if ((typeBits & (1 << memoryTypeIndex)) && ((flags & supportFlags) == flags))
		{
			return memoryTypeIndex;
		}
	}

	NTT_UNREACHABLE();
	return -1;
}

void Buffer::Write(void* pData, u32 size)
{
	void* pBufferData = nullptr;
	VK_ASSERT(vkMapMemory(m_pDevice->GetVkDevice(), m_vkMemory, 0, size, 0, &pBufferData));
	memcpy(pBufferData, pData, size);
	vkUnmapMemory(m_pDevice->GetVkDevice(), m_vkMemory);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN