#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/image.h"
#include "graphics/vulkan/vulkan_command_buffer.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Image::Image(Reference<Device> pDevice, VkImage image, Format format, ImageType type)
	: m_vkImage(image)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_pDevice(pDevice)
	, m_format(format)
	, m_imageType(type)
{
	NTT_ASSERT(image != VK_NULL_HANDLE); // TODO: write the logic for creating image later.
	CreateVkImageView();
}

Image::Image(Image&& other)
	: m_vkImage(other.m_vkImage)
	, m_vkImageView(other.m_vkImageView)
	, m_pDevice(other.m_pDevice)
	, m_format(other.m_format)
	, m_imageType(other.m_imageType)
{
	m_vkImage	  = VK_NULL_HANDLE;
	m_vkImageView = VK_NULL_HANDLE;
	m_pDevice	  = nullptr;
	m_format	  = Format::FORMAT_COUNT;
	m_imageType	  = ImageType::IMAGE_TYPE_2D;
}

void Image::CreateVkImageView()
{
	VkImageViewCreateInfo viewInfo			 = {};
	viewInfo.sType							 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image							 = m_vkImage;
	viewInfo.format							 = getVkFormatFromFormat(m_format);
	viewInfo.viewType						 = getVkImageViewTypeFromImageType(m_imageType);
	viewInfo.subresourceRange.aspectMask	 = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount	 = 1;
	viewInfo.subresourceRange.baseMipLevel	 = 0;
	viewInfo.subresourceRange.levelCount	 = 1;
	viewInfo.components.r					 = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g					 = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b					 = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a					 = VK_COMPONENT_SWIZZLE_IDENTITY;

	VK_ASSERT(vkCreateImageView(m_pDevice->GetVkDevice(), &viewInfo, nullptr, &m_vkImageView));
	m_releaseStack.PushReleaseFunction(
		NTT_NULLPTR, [&](void* pUserData) { vkDestroyImageView(m_pDevice->GetVkDevice(), m_vkImageView, nullptr); });
}

void Image::TransitLayout(CommandBuffer&	   buffer,
						  VkPipelineStageFlags srcStageMask,
						  VkPipelineStageFlags dstStageMask,
						  VkAccessFlags		   srcAccessMask,
						  VkAccessFlags		   dstAccessMask,
						  VkImageLayout		   oldLayout,
						  VkImageLayout		   newLayout)
{
	VkImageSubresourceRange range = {};
	range.aspectMask			  = VK_IMAGE_ASPECT_COLOR_BIT;
	range.baseArrayLayer		  = 0;
	range.layerCount			  = 1;
	range.baseMipLevel			  = 0;
	range.levelCount			  = 1;

	VkImageMemoryBarrier barrier = {};
	barrier.sType				 = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.srcAccessMask		 = srcAccessMask;
	barrier.dstAccessMask		 = dstAccessMask;
	barrier.oldLayout			 = oldLayout;
	barrier.newLayout			 = newLayout;
	barrier.srcQueueFamilyIndex	 = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex	 = VK_QUEUE_FAMILY_IGNORED;
	barrier.image				 = m_vkImage;
	barrier.subresourceRange	 = range;

	vkCmdPipelineBarrier(buffer.GetVkCommandBuffer(),
						 srcStageMask,
						 dstStageMask,
						 VK_DEPENDENCY_BY_REGION_BIT,
						 0,
						 nullptr,
						 0,
						 nullptr,
						 1,
						 &barrier);
}

void Image::ClearImage(CommandBuffer& buffer, Vec4 clearColor)
{
	VkImageSubresourceRange range = {};
	range.aspectMask			  = VK_IMAGE_ASPECT_COLOR_BIT;
	range.baseArrayLayer		  = 0;
	range.layerCount			  = 1;
	range.baseMipLevel			  = 0;
	range.levelCount			  = 1;

	VkClearColorValue value = {};
	value.float32[0]		= clearColor.x;
	value.float32[1]		= clearColor.y;
	value.float32[2]		= clearColor.z;
	value.float32[3]		= clearColor.w;

	vkCmdClearColorImage(
		buffer.GetVkCommandBuffer(), m_vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
}

Image::~Image()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN