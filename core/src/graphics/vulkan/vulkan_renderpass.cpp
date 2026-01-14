#include "graphics/vulkan/vulkan_renderpass.h"
#include "graphics/vulkan/renderer_vulkan.h"
#include "graphics/vulkan/surface_vulkan.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_framebuffer.h"

namespace ntt {

RenderPass::RenderPass(Reference<Device> pDevice, Reference<Surface> pSurface)
	: m_pDevice(pDevice)
	, m_pSurface(pSurface)
	, m_releaseStack()
{
	VkAttachmentDescription attachDesc = {};
	attachDesc.format				   = pSurface->GetSurfaceFormat().format;
	attachDesc.samples				   = VK_SAMPLE_COUNT_1_BIT;
	attachDesc.loadOp				   = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachDesc.storeOp				   = VK_ATTACHMENT_STORE_OP_STORE;
	attachDesc.stencilLoadOp		   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachDesc.stencilStoreOp		   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachDesc.initialLayout		   = VK_IMAGE_LAYOUT_UNDEFINED;
	attachDesc.finalLayout			   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference attachRef = {};
	attachRef.attachment			= 0;
	attachRef.layout				= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass	= {};
	subpass.colorAttachmentCount	= 1;
	subpass.pColorAttachments		= &attachRef;
	subpass.inputAttachmentCount	= 0;
	subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.preserveAttachmentCount = 0;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType				  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount		  = 1;
	renderPassInfo.pAttachments			  = &attachDesc;
	renderPassInfo.subpassCount			  = 1;
	renderPassInfo.pSubpasses			  = &subpass;
	renderPassInfo.dependencyCount		  = 0;

	VK_ASSERT(vkCreateRenderPass(pDevice->GetVkDevice(), &renderPassInfo, nullptr, &m_vkRenderPass));

	m_releaseStack.PushReleaseFunction(
		nullptr, [&](void*) { vkDestroyRenderPass(m_pDevice->GetVkDevice(), m_vkRenderPass, nullptr); });
}

RenderPass::RenderPass(RenderPass&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_pSurface(other.m_pSurface)
	, m_vkRenderPass(other.m_vkRenderPass)
	, m_releaseStack(std::move(other.m_releaseStack))
{
}

void RenderPass::Begin(CommandBuffer& buffer, Framebuffer& framebuffer, const Vec4 color)
{
	VkClearValue clearColor		= {};
	clearColor.color.float32[0] = color.x;
	clearColor.color.float32[1] = color.y;
	clearColor.color.float32[2] = color.z;
	clearColor.color.float32[3] = color.w;

	Vec2 surfaceSize = m_pSurface->GetSize();

	VkRenderPassBeginInfo info	  = {};
	info.sType					  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass				  = m_vkRenderPass;
	info.framebuffer			  = framebuffer.GetVkFramebuffer();
	info.clearValueCount		  = 1;
	info.pClearValues			  = &clearColor;
	info.renderArea.extent.width  = surfaceSize.x;
	info.renderArea.extent.height = surfaceSize.y;
	info.renderArea.offset.x	  = 0;
	info.renderArea.offset.y	  = 0;

	vkCmdBeginRenderPass(buffer.GetVkCommandBuffer(), &info, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(CommandBuffer& buffer)
{
	vkCmdEndRenderPass(buffer.GetVkCommandBuffer());
}

RenderPass::~RenderPass()
{
	m_releaseStack.ReleaseAll();
}

} // namespace ntt
