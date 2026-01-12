#pragma once
#include "platforms/common.h"
#include <vulkan/vulkan.h>

#define VK_ASSERT(exp)                                                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		VkResult result = exp;                                                                                         \
		NTT_ASSERT(result == VK_SUCCESS);                                                                              \
	} while (0)

namespace ntt {

} // namespace ntt
