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

namespace ntt {
class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void AttachSurface(Reference<Surface> pSurface);

	static void BeginFrame();
	static void EndFrame();
	static void PresentFrame();

private:
	static b8				  m_isInitialized;
	static Reference<Surface> s_pSurface;
};

} // namespace ntt
