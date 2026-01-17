#include "ide_application.h"

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
}

void IDEApplication::startEndImpl()
{
}

void IDEApplication::updateBeginImpl(f32 deltaTime)
{
	if (Input::IsKeyPressed(KEY_CODE_ESCAPE))
	{
		NTT_APPLICATION_LOG_INFO("Escape key pressed. Closing the window.");
		Close();
	}
}

void IDEApplication::updateImpl(f32 deltaTime)
{
#if NTT_USE_IMGUI
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Hello, IDE!");
	ImGui::Text("This is the NTT IDE Application using ImGui!");
	ImGui::End();

	ImGui::ShowDemoWindow();
#endif // NTT_USE_IMGUI
}

void IDEApplication::updateEndImpl(f32 deltaTime)
{
}

void IDEApplication::shutdownBeginImpl()
{
}

void IDEApplication::shutdownEndImpl()
{
}

} // namespace ntt