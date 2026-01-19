#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/uniform_buffer.h"
#include "graphics/vulkan/vulkan_buffer.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

UniformBuffer::UniformBuffer(Device* pDevice, u32 size)
	: m_pDevice(pDevice)
{
	m_pBuffer = CreateScope<Buffer>(pDevice,
									size,
									VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
									VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_pBuffer(std::move(other.m_pBuffer))
{
	other.m_pDevice = nullptr;
}

UniformBuffer::~UniformBuffer()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN