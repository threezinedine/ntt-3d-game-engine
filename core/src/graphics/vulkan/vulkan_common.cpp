#if NTT_USE_GRAPHICS_VULKAN
#include "graphics/vulkan/vulkan_common.h"

namespace ntt {
VkImageType getVkImageTypeFromImageType(ImageType type)
{
	switch (type)
	{
	case ImageType::IMAGE_TYPE_1D:
		return VK_IMAGE_TYPE_1D;
	case ImageType::IMAGE_TYPE_2D:
		return VK_IMAGE_TYPE_2D;
	case ImageType::IMAGE_TYPE_3D:
		return VK_IMAGE_TYPE_3D;
	case ImageType::IMAGE_TYPE_COUNT:
		NTT_UNREACHABLE();
	}

	return VK_IMAGE_TYPE_MAX_ENUM;
}

VkImageViewType getVkImageViewTypeFromImageType(ImageType type)
{
	switch (type)
	{
	case ImageType::IMAGE_TYPE_1D:
		return VK_IMAGE_VIEW_TYPE_1D;
	case ImageType::IMAGE_TYPE_2D:
		return VK_IMAGE_VIEW_TYPE_2D;
	case ImageType::IMAGE_TYPE_3D:
		return VK_IMAGE_VIEW_TYPE_3D;
	case ImageType::IMAGE_TYPE_COUNT:
		NTT_UNREACHABLE();
	}

	return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
}

VkFormat getVkFormatFromFormat(Format format)
{
	switch (format)
	{
	case Format::FORMAT_R8G8B8A8_SRGB:
		return VK_FORMAT_R8G8B8A8_SRGB;
	case Format::FORMAT_B8G8R8A8_SRGB:
		return VK_FORMAT_B8G8R8A8_SRGB;
	case Format::FORMAT_COUNT:
		NTT_UNREACHABLE();
	}

	return VK_FORMAT_MAX_ENUM;
}

Format getFormatFromVkFormat(VkFormat format)
{
	switch (format)
	{
	case VK_FORMAT_R8G8B8A8_SRGB:
		return Format::FORMAT_R8G8B8A8_SRGB;
	case VK_FORMAT_B8G8R8A8_SRGB:
		return Format::FORMAT_B8G8R8A8_SRGB;
	default:
		NTT_UNREACHABLE();
	}

	return Format::FORMAT_COUNT;
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN