#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {

class Device;
class ReleaseStack;
class CommandPool;

class CommandBuffer
{
public:
	CommandBuffer(Device* pDevice, CommandPool* pPool, VkCommandBuffer buffer);
	CommandBuffer(const CommandBuffer& other);
	CommandBuffer(CommandBuffer&& other) noexcept;
	~CommandBuffer();

	inline VkCommandBuffer& GetVkCommandBuffer()
	{
		return m_vkCommandBuffer;
	}

public:
	void Reset();
	void StartRecord();

	void EndRecord();

private:
	Device*			m_pDevice;
	CommandPool*	m_pPool;
	VkCommandBuffer m_vkCommandBuffer;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN