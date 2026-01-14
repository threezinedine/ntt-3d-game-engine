#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_common.h"
#include "platforms/common.h"

namespace ntt {
class Device;
class Image;
class RenderPass;
class Surface;
class ReleaseStack;

class Framebuffer
{
public:
	Framebuffer(Reference<Device> pDevice, RenderPass* pRenderPass, Surface* pSurface, Image* pImage);
	NTT_DELETE_COPY(Framebuffer)
	Framebuffer(Framebuffer&& other) noexcept;
	~Framebuffer();

public:
	inline VkFramebuffer& GetVkFramebuffer()
	{
		return m_vkFramebuffer;
	}

private:
	Reference<Device> m_pDevice;
	RenderPass*		  m_pRenderPass;
	Surface*		  m_pSurface;
	Image*			  m_pImage;
	VkFramebuffer	  m_vkFramebuffer;

private:
	ReleaseStack m_releaseStack;
};
} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN