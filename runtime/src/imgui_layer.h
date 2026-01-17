#if NTT_USE_IMGUI
#pragma once
#include <engine/engine.h>

namespace ntt {

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	NTT_DELETE_COPY(ImGuiLayer)
	ImGuiLayer(ImGuiLayer&& other);
	virtual ~ImGuiLayer();

public:
	virtual void updateImpl(f32 deltaTime) override;
};

} // namespace ntt

#endif // NTT_USE_IMGUI