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
	InitializeWindowingSystem();

	m_pWindow = CreateRef<Window>("NTT Engine Window", 1280, 720);

	m_pWindow->Initialize();
}

b8 RuntimeApplication::IsOpen() const
{
	return m_pWindow->IsOpen();
}

void RuntimeApplication::startEndImpl()
{
	Renderer::AttachSurface(m_pWindow->GetSurface());
}

void RuntimeApplication::updateBeginImpl(f32 deltaTime)
{
	m_pWindow->PollEvents();

	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		m_pWindow->Close();
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
	m_pWindow->Shutdown();

	ShutdownWindowingSystem();
}

} // namespace ntt

#endif // !NTT_ENGINE_EDITOR_BINDING && NTT_NO_ANALYZE && !NTT_IDE_APPLICATION
