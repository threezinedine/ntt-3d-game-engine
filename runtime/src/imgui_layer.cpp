#if NTT_USE_IMGUI
#include "imgui_layer.h"

namespace ntt {

ImGuiLayer::ImGuiLayer()
	: Layer()
{
}

ImGuiLayer::ImGuiLayer(ImGuiLayer&& other)
	: Layer(std::move(other))
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::updateImpl(f32 deltaTime)
{
	ImGui::Begin("ImGui Layer");
	ImGui::Text("Hello from ImGui Layer!");
	ImGui::End();
}

} // namespace ntt

#endif // NTT_USE_IMGUI