#include "runtime_application.h"
#include "imgui_layer.h"

namespace ntt {

RuntimeApplication::RuntimeApplication()
	: Application()
{
#if NTT_USE_IMGUI
	m_pLayers.push_back(std::move(CreateScope<ImGuiLayer>())); // ImGui layer must be the last one

#endif // NTT_USE_IMGUI
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

#if NTT_USE_IMGUI
	if (Input::IsKeyPressed(KEY_CODE_F12))
	{
		Scope<Layer>& imguiLayer = m_pLayers.back(); // The imgui layer is always the last one
		if (imguiLayer->IsAttached())
		{
			disableLayer(m_pLayers.size() - 1); // The imgui layer is always the last one
		}
		else
		{
			enableLayer(m_pLayers.size() - 1); // The imgui layer is always the last one
		}
	}
#endif // NTT_USE_IMGUI
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
