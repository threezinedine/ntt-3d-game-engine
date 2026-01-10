#pragma once
#include "platforms/common.h"
#include "platforms/platforms.h"

namespace ntt {
class Renderer
{
public:
	static void Initialize();
	static void Shutdown();

	static void AttachSurface(Ref<Surface>& surface);

private:
	static b8 m_isInitialized;
};

} // namespace ntt
