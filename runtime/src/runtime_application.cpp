#include "runtime_application.h"

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
