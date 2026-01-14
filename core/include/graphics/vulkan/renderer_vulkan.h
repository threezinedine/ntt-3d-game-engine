#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {

struct QueueFamily;
class Device;
class Swapchain;
class Semaphore;
class Fence;
class CommandBuffer;
class RenderPass;
class Framebuffer;

class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void AttachSurface(Reference<Surface> pSurface);

	static void BeginFrame();
	static void EndFrame();
	static void PresentFrame();

public:
	static inline VkInstance GetVkInstance()
	{
		return s_vkInstance;
	}

	static inline VkPhysicalDevice GetVkPhysicalDevice()
	{
		return s_vkPhysicalDevice;
	}

	static inline Reference<Device> GetDevice()
	{
		return s_pDevice;
	}

	static i32 GetRenderQueueFamilyIndex();
	static i32 GetComputeQueueFamilyIndex();
	static i32 GetTransferQueueFamilyIndex();

private:
	static void CreateInstance(const Array<const char*>& extensions, const Array<const char*>& layers);
	static void ChoosePhysicalDevice();
	static void ChooseQueueFamilies();
	static void CheckingTheSurfaceSupport();
	static void LoadingDefaultShader();
	static void CreateSyncObjects();
	static void CreateFrameBuffers();

private:
#if NTT_DEBUG
	static void CreateDebugUtilsMessenger();
#endif // NTT_DEBUG

private:
	static b8				  m_isInitialized;
	static Reference<Surface> s_pSurface;

private:
	static VkInstance		 s_vkInstance;
	static VkPhysicalDevice	 s_vkPhysicalDevice;
	static Reference<Device> s_pDevice;
	static Scope<Swapchain>	 s_pSwapchain;

	static QueueFamily s_renderQueueFamily;
	static QueueFamily s_computeQueueFamily;
	static QueueFamily s_transferQueueFamily;

private:
	static Scope<RenderPass> s_pRenderPass;

private:
	static Array<Fence>			s_fences;
	static Array<Semaphore>		s_imageReadySemaphores;
	static Array<Semaphore>		s_renderFinisedSemaphores;
	static Array<CommandBuffer> s_renderCommandBuffers;
	static Array<CommandBuffer> s_presentCommandBuffers;
	static Array<Framebuffer>	s_framebuffers;

private:
	static u32 s_currentFlight;

private:
#if NTT_DEBUG
	static VkDebugUtilsMessengerEXT s_vkDebugMessenger;
#endif // NTT_DEBUG

private:
	static ReleaseStack s_releaseStack;
};

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN