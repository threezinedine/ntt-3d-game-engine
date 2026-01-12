#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "containers/containers.h"
#include "platforms/common.h"
#include "platforms/platforms.h"
#include "vulkan_common.h"

namespace ntt {
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

private:
	static void CreateInstance();

private:
	static b8				  m_isInitialized;
	static Reference<Surface> s_pSurface;

private:
	static VkInstance s_vkInstance;

private:
	static ReleaseStack s_releaseStack;
};

} // namespace ntt
#endif // NTT_USE_GRAPHICS_VULKAN