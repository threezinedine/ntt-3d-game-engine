#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {

class Device;
class ReleaseStack;
class CommandBuffer;
class Framebuffer;

class RenderPass
{
public:
	RenderPass(Reference<Device> pDevice, Reference<Surface> pSurface);
	NTT_DELETE_COPY(RenderPass);
	RenderPass(RenderPass&& other) noexcept;
	~RenderPass();

	VkRenderPass GetVkRenderPass() const
	{
		return m_vkRenderPass;
	}

	void Begin(CommandBuffer& buffer, Framebuffer& framebuffer, const Vec4 color);
	void End(CommandBuffer& buffer);

private:
	Reference<Device>  m_pDevice;
	Reference<Surface> m_pSurface;
	VkRenderPass	   m_vkRenderPass;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN