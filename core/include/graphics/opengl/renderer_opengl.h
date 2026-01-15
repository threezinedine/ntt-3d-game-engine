#if NTT_USE_GRAPHICS_OPENGL
#pragma once
#include "opengl_common.h"
#include "platforms/platforms.h"

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

#endif // NTT_USE_GRAPHICS_OPENGL