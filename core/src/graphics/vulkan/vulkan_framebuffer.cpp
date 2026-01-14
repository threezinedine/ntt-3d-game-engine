#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_framebuffer.h"
#include "graphics/vulkan/image_vulkan.h"
#include "graphics/vulkan/surface_vulkan.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_renderpass.h"

namespace ntt {

Framebuffer::Framebuffer(Reference<Device> pDevice, RenderPass* pRenderPass, Surface* pSurface, Image* pImage)
	: m_pDevice(pDevice)
	, m_pRenderPass(pRenderPass)
	, m_pSurface(pSurface)
	, m_pImage(pImage)
{
	VkFramebufferCreateInfo bufferInfo = {};
	bufferInfo.sType				   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	bufferInfo.width				   = pSurface->GetSize().x;
	bufferInfo.height				   = pSurface->GetSize().y;
	bufferInfo.renderPass			   = pRenderPass->GetVkRenderPass();
	bufferInfo.layers				   = 1;
	bufferInfo.pAttachments			   = &pImage->GetVkImageView();
	bufferInfo.attachmentCount		   = 1;

	VK_ASSERT(vkCreateFramebuffer(pDevice->GetVkDevice(), &bufferInfo, nullptr, &m_vkFramebuffer));

	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyFramebuffer(m_pDevice->GetVkDevice(), m_vkFramebuffer, nullptr); });
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_pRenderPass(other.m_pRenderPass)
	, m_pImage(other.m_pImage)
	, m_releaseStack(std::move(m_releaseStack))
{
	other.m_pDevice		  = nullptr;
	other.m_pRenderPass	  = nullptr;
	other.m_pImage		  = nullptr;
	other.m_vkFramebuffer = VK_NULL_HANDLE;
}

Framebuffer::~Framebuffer()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN