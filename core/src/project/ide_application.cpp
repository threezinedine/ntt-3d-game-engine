#if NTT_IDE_APPLICATION

#include "project/ide_application.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "platforms/graphics/graphics.h"

namespace ntt {

IDEApplication::IDEApplication()
	: Application()
{
}

IDEApplication::~IDEApplication()
{
}

void IDEApplication::startBeginImpl()
{
	InitializeWindowingSystem();
	m_pWindow = CreateScope<Window>("NTT IDE Window", 1280, 720);
	m_pWindow->Initialize();
}

void IDEApplication::startEndImpl()
{
	Renderer::AttachSurface(m_pWindow->GetSurface());
}

void IDEApplication::updateBeginImpl(f32 deltaTime)
{
	m_pWindow->PollEvents();

	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		m_pWindow->Close();
	}
}

void IDEApplication::updateEndImpl(f32 deltaTime)
{
}

void IDEApplication::shutdownBeginImpl()
{
}

void IDEApplication::shutdownEndImpl()
{
	m_pWindow->Shutdown();
	ShutdownWindowingSystem();
}

b8 IDEApplication::IsOpen() const
{
	return m_pWindow->IsOpen();
}

} // namespace ntt

#endif // NTT_IDE_APPLICATION