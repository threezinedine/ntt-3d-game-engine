#pragma once
#include "containers/containers.h"
#include "graphics/renderer.h"
#include "platforms/common.h"
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

} // namespace ntt
