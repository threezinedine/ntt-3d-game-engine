#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"
#include "vulkan_common.h"
#include <vulkan/vulkan.h>

namespace ntt {
class Device;
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

	static inline Device& GetDevice()
	{
		return s_device;
	}

private:
	static void CreateInstance(const Array<const char*>& extensions, const Array<const char*>& layers);
	static void ChoosePhysicalDevice();
	static void ChooseQueueFamilies();
	static void CreateDevice(const Array<const char*>& extensions, const Array<const char*>& layers);
	static void CheckingTheSurfaceSupport();

private:
	static b8				  m_isInitialized;
	static Reference<Surface> s_pSurface;

private:
	static VkInstance		s_vkInstance;
	static VkPhysicalDevice s_vkPhysicalDevice;
	static Device			s_device;

	static QueueFamily s_renderQueueFamily;
	static QueueFamily s_computeQueueFamily;
	static QueueFamily s_transferQueueFamily;

private:
	static ReleaseStack s_releaseStack;
};

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN