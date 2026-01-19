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
{
	if (type == VertexBufferType::VERTEX_BUFFER_TYPE_STATIC)
	{
		m_pBuffer = CreateScope<Buffer>(m_pDevice,
										size,
										VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
										VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		m_pLocalBuffer = CreateScope<Buffer>(m_pDevice,
											 size,
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

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_type(other.m_type)
	, m_pBuffer(std::move(other.m_pBuffer))
	, m_pLocalBuffer(std::move(other.m_pLocalBuffer))
{
	other.m_pDevice = nullptr;
	other.m_type	= VertexBufferType::VERTEX_BUFFER_TYPE_COUNT;
}

VertexBuffer::~VertexBuffer()
{
	m_pBuffer.reset();

	if (m_pLocalBuffer)
	{
		m_pLocalBuffer.reset();
	}
}

void VertexBuffer::Write(CommandBuffer& buffer, void* pData, u32 size)
{
	m_pBuffer->Write(pData, size);

	buffer.StartRecord(true);

	VkBufferCopy region = {};
	region.srcOffset	= 0;
	region.dstOffset	= 0;
	region.size			= size;

	vkCmdCopyBuffer(buffer.GetVkCommandBuffer(), m_pBuffer->GetVkBuffer(), m_pLocalBuffer->GetVkBuffer(), 1, &region);

	buffer.EndRecord();

	m_pDevice->GetRenderQueue()->SubmitSync(buffer);
	m_pDevice->GetRenderQueue()->Wait();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN