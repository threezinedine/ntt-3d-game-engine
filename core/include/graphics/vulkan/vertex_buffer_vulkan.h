#if NTT_USE_GRAPHICS_VULKAN
#pragma once

#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {

class Device;
class CommandBuffer;

class VertexBuffer
{
public:
	VertexBuffer(Device* pDevice, u32 size, VertexBufferType type = VertexBufferType::VERTEX_BUFFER_TYPE_STATIC);
	NTT_DELETE_COPY(VertexBuffer)
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

public:
	void Write(CommandBuffer& buffer, void* pData, u32 size);

	inline VkBuffer& GetVkBuffer()
	{
		return m_vkBuffer;
	}

	inline VkBuffer& GetVkLocalBuffer()
	{
		return m_vkLocalBuffer;
	}

private:
	void CreateBuffer(
		VkBuffer* pBuffer, u32 size, VkDeviceMemory* pMemory, VkBufferUsageFlags usage, VkMemoryPropertyFlags flags);
	i32 GetMemoryTypeIndex(u32 typeBits, VkMemoryPropertyFlags flags);

private:
	Device*			 m_pDevice;
	VertexBufferType m_type;
	VkBuffer		 m_vkBuffer;
	VkDeviceMemory	 m_vkMemory;
	u32				 m_actualSize;

private:
	VkBuffer	   m_vkLocalBuffer; // only be used for static buffer
	VkDeviceMemory m_vkLocalMemory;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN