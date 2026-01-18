#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/vulkan/vulkan_queue.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"
#include "graphics/vulkan/vulkan_fence.h"
#include "graphics/vulkan/vulkan_semaphore.h"
#include "graphics/vulkan/vulkan_swapchain.h"

namespace ntt {

GraphicQueue::GraphicQueue(Device* pDevice, VkQueue vkQueue)
	: m_pDevice(pDevice)
	, m_vkQueue(vkQueue)
{
}

GraphicQueue::GraphicQueue(const GraphicQueue& other)
	: m_pDevice(other.m_pDevice)
	, m_vkQueue(other.m_vkQueue)
{
}

GraphicQueue::GraphicQueue(GraphicQueue&& other) noexcept
	: m_pDevice(other.m_pDevice)
	, m_vkQueue(other.m_vkQueue)
{
	other.m_pDevice = nullptr;
	other.m_vkQueue = VK_NULL_HANDLE;
}

GraphicQueue::~GraphicQueue()
{
}

void GraphicQueue::SubmitSync(CommandBuffer& buffer)
{
	VkSubmitInfo submitInfo		  = {};
	submitInfo.sType			  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers	  = &buffer.GetVkCommandBuffer();

	vkQueueSubmit(m_vkQueue, 1, &submitInfo, VK_NULL_HANDLE);
}

void GraphicQueue::SubmitRender(CommandBuffer& buffer,
								Semaphore&	   waitSemaphore,
								Semaphore&	   signalSemaphore,
								Fence&		   fence)
{
	VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo			= {};
	submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount	= 1;
	submitInfo.pCommandBuffers		= &buffer.GetVkCommandBuffer();
	submitInfo.waitSemaphoreCount	= 1;
	submitInfo.pWaitSemaphores		= &waitSemaphore.GetVkSemaphore();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores	= &signalSemaphore.GetVkSemaphore();
	submitInfo.pWaitDstStageMask	= &dstStage;

	VK_ASSERT(vkQueueSubmit(m_vkQueue, 1, &submitInfo, fence.GetVkFence()));
}

void GraphicQueue::SubmitPresent(Semaphore& waitSemaphore, Scope<Swapchain>& pSwapchain)
{
	u32 currentImageIndex = pSwapchain->GetCurrentImageIndex();

	VkPresentInfoKHR presentInfo   = {};
	presentInfo.sType			   = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pImageIndices	   = &currentImageIndex;
	presentInfo.pSwapchains		   = &pSwapchain->GetVkSwapchain();
	presentInfo.swapchainCount	   = 1;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores	   = &waitSemaphore.GetVkSemaphore();

	VK_ASSERT(vkQueuePresentKHR(m_vkQueue, &presentInfo));
}

void GraphicQueue::Wait()
{
	vkQueueWaitIdle(m_vkQueue);
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN