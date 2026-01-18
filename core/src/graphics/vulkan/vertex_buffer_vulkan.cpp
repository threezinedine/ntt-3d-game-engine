#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vertex_buffer_vulkan.h"
#include "graphics/vulkan/renderer_vulkan.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_queue.h"

namespace ntt {

VertexBuffer::VertexBuffer(Device* pDevice, u32 size, VertexBufferType type)
	: m_pDevice(pDevice)
	, m_type(type)
	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkMemory(VK_NULL_HANDLE)
	, m_vkLocalBuffer(VK_NULL_HANDLE)
	, m_vkLocalMemory(VK_NULL_HANDLE)
{
	if (type == VertexBufferType::VERTEX_BUFFER_TYPE_STATIC)
	{
		CreateBuffer(&m_vkBuffer,
					 size,
					 &m_vkMemory,
					 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		CreateBuffer(&m_vkLocalBuffer,
					 size,
					 &m_vkLocalMemory,
					 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
					 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}
	else if (type == VertexBufferType::VERTEX_BUFFER_TYPE_STATIC)
	{
		NTT_UNIMPLEMENTED();
	}
	else
	{
		NTT_UNREACHABLE();
	}
}

void VertexBuffer::CreateBuffer(
	VkBuffer* pBuffer, u32 size, VkDeviceMemory* pMemory, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType			  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.usage			  = usage;
	bufferInfo.sharingMode		  = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.size				  = size;

	VK_ASSERT(vkCreateBuffer(m_pDevice->GetVkDevice(), &bufferInfo, nullptr, pBuffer));

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetVkDevice(), *pBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType				  = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize		  = memoryRequirements.size;
	allocateInfo.memoryTypeIndex	  = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits, flags);

	VK_ASSERT(vkAllocateMemory(m_pDevice->GetVkDevice(), &allocateInfo, nullptr, pMemory));

	VkDeviceSize offset = 0;

	VK_ASSERT(vkBindBufferMemory(m_pDevice->GetVkDevice(), *pBuffer, *pMemory, offset));
}

i32 VertexBuffer::GetMemoryTypeIndex(u32 typeBits, VkMemoryPropertyFlags flags)
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

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_type(other.m_type)
	, m_vkBuffer(other.m_vkBuffer)
	, m_vkMemory(other.m_vkMemory)
	, m_vkLocalBuffer(other.m_vkLocalBuffer)
	, m_vkLocalMemory(other.m_vkLocalMemory)
{
	other.m_pDevice		  = nullptr;
	other.m_type		  = VertexBufferType::VERTEX_BUFFER_TYPE_COUNT;
	other.m_vkBuffer	  = VK_NULL_HANDLE;
	other.m_vkMemory	  = VK_NULL_HANDLE;
	other.m_vkLocalBuffer = VK_NULL_HANDLE;
	other.m_vkLocalMemory = VK_NULL_HANDLE;
}

VertexBuffer::~VertexBuffer()
{
	if (m_vkLocalMemory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_pDevice->GetVkDevice(), m_vkLocalMemory, nullptr);
	}

	if (m_vkLocalBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_pDevice->GetVkDevice(), m_vkLocalBuffer, nullptr);
	}

	vkFreeMemory(m_pDevice->GetVkDevice(), m_vkMemory, nullptr);
	vkDestroyBuffer(m_pDevice->GetVkDevice(), m_vkBuffer, nullptr);
}

void VertexBuffer::Write(CommandBuffer& buffer, void* pData, u32 size)
{
	void* pBufferData = nullptr;
	NTT_ASSERT(pBufferData == nullptr);

	VK_ASSERT(vkMapMemory(m_pDevice->GetVkDevice(), m_vkMemory, 0, size, 0, &pBufferData));
	NTT_ASSERT(pBufferData != nullptr);
	u8* pDst = static_cast<u8*>(pBufferData);
	memcpy(pDst, pData, size);
	vkUnmapMemory(m_pDevice->GetVkDevice(), m_vkMemory);

	buffer.StartRecord(true);

	VkBufferCopy region = {};
	region.srcOffset	= 0;
	region.dstOffset	= 0;
	region.size			= size;

	vkCmdCopyBuffer(buffer.GetVkCommandBuffer(), m_vkBuffer, m_vkLocalBuffer, 1, &region);

	buffer.EndRecord();

	m_pDevice->GetRenderQueue()->SubmitSync(buffer);
	m_pDevice->GetRenderQueue()->Wait();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN