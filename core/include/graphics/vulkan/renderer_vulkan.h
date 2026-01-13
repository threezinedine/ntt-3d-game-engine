#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {
class Device;
class Swapchain;
struct QueueFamily;

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

	static inline Device& GetDevice()
	{
		return s_device;
	}

	static u32 GetRenderQueueFamilyIndex();

private:
	static void CreateInstance(const Array<const char*>& extensions, const Array<const char*>& layers);
	static void ChoosePhysicalDevice();
	static void ChooseQueueFamilies();
	static void CreateDevice(const Array<const char*>& extensions, const Array<const char*>& layers);
	static void CheckingTheSurfaceSupport();
	static void CreateSyncObjects();

private:
#if NTT_DEBUG
	static void CreateDebugUtilsMessenger();
#endif // NTT_DEBUG

private:
	static b8				  m_isInitialized;
	static Reference<Surface> s_pSurface;

private:
	static VkInstance		s_vkInstance;
	static VkPhysicalDevice s_vkPhysicalDevice;
	static Device			s_device;
	static Scope<Swapchain> s_pSwapchain;

	static QueueFamily s_renderQueueFamily;
	static QueueFamily s_computeQueueFamily;
	static QueueFamily s_transferQueueFamily;

private:
	static Array<VkFence>	  s_fences;
	static Array<VkSemaphore> s_imageReadySemaphores;
	static Array<VkSemaphore> s_renderFinisedSemaphores;

private:
#if NTT_DEBUG
	static VkDebugUtilsMessengerEXT s_vkDebugMessenger;
#endif // NTT_DEBUG

private:
	static ReleaseStack s_releaseStack;
};

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN