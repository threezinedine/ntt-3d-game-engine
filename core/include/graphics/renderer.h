#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"

namespace ntt {
class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void AttachSurface(Ref<Surface>& pSurface);

	static void BeginFrame();
	static void EndFrame();
	static void PresentFrame();

private:
	static b8			m_isInitialized;
	static Ref<Surface> s_pSurface;
};

} // namespace ntt
