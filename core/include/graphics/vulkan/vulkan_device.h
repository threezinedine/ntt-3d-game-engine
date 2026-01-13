#if NTT_USE_GRAPHICS_VULKAN
#pragma once

#include "platforms/common.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {

class GraphicQueue;
class CommandBuffer;
class Surface;
class CommandPool;

class Device
{
public:
	Device(VkPhysicalDevice			 vkPhysicalDevice,
		   Reference<Surface>		 pSurface,
		   const Array<const char*>& extensions,
		   const Array<const char*>& layers);
	NTT_DELETE_COPY(Device)
	NTT_DELETE_MOVE(Device)
	~Device();

public:
	inline b8 IsInitialized()
	{
		return m_vkDevice != VK_NULL_HANDLE;
	}

	inline VkPhysicalDevice GetVkPhysicalDevice() const
	{
		return m_vkPhysicalDevice;
	}

	inline VkDevice& GetVkDevice()
	{
		return m_vkDevice;
	}

	inline Scope<GraphicQueue>& GetRenderQueue()
	{
		return m_pRenderQueue;
	}

	inline Scope<GraphicQueue>& GetPresentQueue()
	{
		return m_pPresentQueue;
	}

	inline Scope<GraphicQueue>& GetComputeQueue()
	{
		return m_pComputeQueue;
	}

	inline Scope<GraphicQueue>& GetTransferQueue()
	{
		return m_pTransferQueue;
	}

	inline Scope<CommandPool>& GetRenderCommandPool()
	{
		return m_pRenderCommandPool;
	}

	inline Scope<CommandPool>& GetPresentCommandPool()
	{
		return m_pPresentCommandPool;
	}

	inline Scope<CommandPool>& GetComputeCommandPool()
	{
		return m_pComputeCommandPool;
	}

	inline Scope<CommandPool>& GetTransferCommandPool()
	{
		return m_pTransferCommandPool;
	}

private:
	VkPhysicalDevice	m_vkPhysicalDevice;
	Reference<Surface>	m_pSurface;
	VkDevice			m_vkDevice;
	Scope<GraphicQueue> m_pRenderQueue;
	Scope<GraphicQueue> m_pPresentQueue;
	Scope<GraphicQueue> m_pComputeQueue;
	Scope<GraphicQueue> m_pTransferQueue;
	Scope<CommandPool>	m_pRenderCommandPool;
	Scope<CommandPool>	m_pPresentCommandPool;
	Scope<CommandPool>	m_pComputeCommandPool;
	Scope<CommandPool>	m_pTransferCommandPool;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN