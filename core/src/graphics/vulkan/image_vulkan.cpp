#if NTT_USE_GRAPHICS_VULKAN

#include "graphics/image.h"
#include "graphics/vulkan/vulkan_device.h"

namespace ntt {

Image::Image(Device* pDevice, VkImage image, Format format, ImageType type)
	: m_vkImage(image)
	, m_vkImageView(VK_NULL_HANDLE)
	, m_pDevice(pDevice)
	, m_format(format)
	, m_imageType(type)
{
	NTT_ASSERT(image != VK_NULL_HANDLE); // TODO: write the logic for creating image later.
	CreateVkImageView();
}

void Image::CreateVkImageView()
{
	VkImageViewCreateInfo viewInfo			 = {};
	viewInfo.sType							 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image							 = m_vkImage;
	viewInfo.format							 = getVkFormatFromFormat(m_format);
	viewInfo.viewType						 = getVkImageViewTypeFromImageType(m_imageType);
	viewInfo.subresourceRange.aspectMask	 = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;
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

Image::~Image()
{
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN