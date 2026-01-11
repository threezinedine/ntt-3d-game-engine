#if NTT_ENGINE_EDITOR_BINDING

#include "project/editor_application.h"
#include "graphics/graphics.h"

namespace ntt {

EditorApplication::EditorApplication(const String& projectFilePath)
	: Application(projectFilePath)
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::startBeginImpl()
{
	Renderer::Initialize();
}

void EditorApplication::startEndImpl()
{
	Renderer::AttachSurface(nullptr);
}

void EditorApplication::updateBeginImpl(f32 deltaTime)
{
}

void EditorApplication::updateEndImpl(f32 deltaTime)
{
}

void EditorApplication::shutdownBeginImpl()
{
}

void EditorApplication::shutdownEndImpl()
{
	Renderer::Shutdown();
}

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING