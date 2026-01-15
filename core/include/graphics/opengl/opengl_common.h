#if NTT_USE_GRAPHICS_OPENGL
#pragma once
#include "graphics/types.h"
#include "platforms/common.h"
// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "utils/logger/logger.h"

#define GL_ASSERT(exp)                                                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		while (glGetError() != GL_NO_ERROR);                                                                           \
		exp;                                                                                                           \
		GLenum error = glGetError();                                                                                   \
		if (error != GL_NO_ERROR)                                                                                      \
		{                                                                                                              \
			NTT_OPENGL_LOG_FATAL("OpenGL call failed with error code: %u", error);                                     \
			NTT_UNREACHABLE();                                                                                         \
		}                                                                                                              \
	} while (0)

#define NTT_OPENGL_LOG_TRACE(message, ...) NTT_LOG_TRACE(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)
#define NTT_OPENGL_LOG_DEBUG(message, ...) NTT_LOG_DEBUG(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)
#define NTT_OPENGL_LOG_INFO(message, ...)  NTT_LOG_INFO(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)
#define NTT_OPENGL_LOG_WARN(message, ...)  NTT_LOG_WARN(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)
#define NTT_OPENGL_LOG_ERROR(message, ...) NTT_LOG_ERROR(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)
#define NTT_OPENGL_LOG_FATAL(message, ...) NTT_LOG_FATAL(LogTagMaskBit::LOG_TAG_MASK_OPENGL, message, ##__VA_ARGS__)

#endif // NTT_USE_GRAPHICS_OPENGL