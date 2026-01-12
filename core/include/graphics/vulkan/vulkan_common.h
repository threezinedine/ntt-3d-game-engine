#if NTT_USE_GRAPHICS_VULKAN
#pragma once
#include "containers/containers.h"
#include "graphics/renderer.h"
#include "graphics/types.h"
#include "platforms/common.h"
#include "utils/logger/logger.h"
#include <vulkan/vulkan.h>

#define VK_ASSERT(exp)                                                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		VkResult result = exp;                                                                                         \
		if (result != VK_SUCCESS)                                                                                      \
		{                                                                                                              \
			NTT_SYSTEM_LOG_FATAL("Vulkan call failed with error code: %u", result);                                    \
			NTT_UNREACHABLE();                                                                                         \
		}                                                                                                              \
	} while (0)

namespace ntt {
struct QueueFamily
{
	u32 familyIndex;
	b8	exist;
};

VkImageType		getVkImageTypeFromImageType(ImageType type);
VkImageViewType getVkImageViewTypeFromImageType(ImageType type);
VkFormat		getVkFormatFromFormat(Format format);

Format getFormatFromVkFormat(VkFormat format);

#define NTT_VULKAN_LOG_TRACE(message, ...) NTT_LOG_TRACE(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)
#define NTT_VULKAN_LOG_DEBUG(message, ...) NTT_LOG_DEBUG(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)
#define NTT_VULKAN_LOG_INFO(message, ...)  NTT_LOG_INFO(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)
#define NTT_VULKAN_LOG_WARN(message, ...)  NTT_LOG_WARN(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)
#define NTT_VULKAN_LOG_ERROR(message, ...) NTT_LOG_ERROR(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)
#define NTT_VULKAN_LOG_FATAL(message, ...) NTT_LOG_FATAL(LogTagMaskBit::LOG_TAG_MASK_VULKAN, message, ##__VA_ARGS__)

} // namespace ntt

#endif // NTT_USE_GRAPHICS_VULKAN