#if !NTT_ENGINE_EDITOR_BINDING && NTT_NO_ANALYZE && !NTT_IDE_APPLICATION

#include "project/runtime_application.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "platforms/graphics/graphics.h"

namespace ntt {

RuntimeApplication::RuntimeApplication()
	: Application()
{
}

RuntimeApplication::~RuntimeApplication()
{
}

void RuntimeApplication::startBeginImpl()
{
}

void RuntimeApplication::startEndImpl()
{
}

void RuntimeApplication::updateBeginImpl(f32 deltaTime)
{
	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		Close();
	}
}

void RuntimeApplication::updateEndImpl(f32 deltaTime)
{
}

void RuntimeApplication::shutdownBeginImpl()
{
}

void RuntimeApplication::shutdownEndImpl()
{
}

} // namespace ntt

#endif // !NTT_ENGINE_EDITOR_BINDING && NTT_NO_ANALYZE && !NTT_IDE_APPLICATION
