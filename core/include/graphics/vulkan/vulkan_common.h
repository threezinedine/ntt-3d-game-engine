#pragma once
#include "containers/containers.h"
#include "graphics/renderer.h"
#include "platforms/common.h"
#include <vulkan/vulkan.h>

#define VK_ASSERT(exp)                                                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		VkResult result = exp;                                                                                         \
		NTT_ASSERT(result == VK_SUCCESS);                                                                              \
	} while (0)

namespace ntt {
struct QueueFamily
{
	u32 familyIndex;
	b8	exist;
};

} // namespace ntt
