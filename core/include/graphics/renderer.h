#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"
#include "utils/logger/logger.h"

#define NTT_RENDERER_LOG_TRACE(message, ...) NTT_LOG_TRACE(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)
#define NTT_RENDERER_LOG_DEBUG(message, ...) NTT_LOG_DEBUG(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)
#define NTT_RENDERER_LOG_INFO(message, ...)	 NTT_LOG_INFO(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)
#define NTT_RENDERER_LOG_WARN(message, ...)	 NTT_LOG_WARN(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)
#define NTT_RENDERER_LOG_ERROR(message, ...) NTT_LOG_ERROR(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)
#define NTT_RENDERER_LOG_FATAL(message, ...) NTT_LOG_FATAL(LogTagMaskBit::LOG_TAG_MASK_RENDERER, message, ##__VA_ARGS__)

#include "opengl/renderer_opengl.h"
#include "vulkan/renderer_vulkan.h"