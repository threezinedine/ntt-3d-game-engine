#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {

class Device;
class ReleaseStack;
class CommandBuffer;

class CommandPool
{
public:
	CommandPool(Device* pDevice, u32 familyIndex);
	NTT_DELETE_COPY(CommandPool)
	NTT_DELETE_MOVE(CommandPool)
	~CommandPool();

	inline VkCommandPool GetVkCommandPool()
	{
		return m_vkPool;
	}

	Array<CommandBuffer> AllocateCommandBuffers(u32 count);
	void				 FreeCommandBuffers(u32 count, CommandBuffer* buffers);

private:
	Device*		  m_pDevice;
	VkCommandPool m_vkPool;
	u32			  m_familyIndex;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN
