#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "containers/release_stack.h"
#include "platforms/common.h"
#include "vulkan_common.h"

namespace ntt {
class Device;
class CommandBuffer;

class Image
{
public:
	Image(Reference<Device> pDevice,
		  VkImage			image  = VK_NULL_HANDLE,
		  Format			format = Format::FORMAT_COUNT,
		  ImageType			type   = ImageType::IMAGE_TYPE_2D);
	Image(const Image& other);
	Image(Image&& other);
	~Image();

public:
	void ClearImage(CommandBuffer& buffer, Vec4 clearColor);

	void TransitLayout(CommandBuffer&		buffer,
					   VkPipelineStageFlags srcStageMask,
					   VkPipelineStageFlags dstStageMask,
					   VkAccessFlags		srcAccessMask,
					   VkAccessFlags		dstAccessMask,
					   VkImageLayout		oldLayout,
					   VkImageLayout		newLayout);

private:
	void CreateVkImageView();

private:
	VkImage			  m_vkImage;
	VkImageView		  m_vkImageView;
	Reference<Device> m_pDevice;
	Format			  m_format;
	ImageType		  m_imageType;

private:
	ReleaseStack m_releaseStack;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN