#if NTT_ENGINE_EDITOR_BINDING

#include "project/editor_application.h"

namespace ntt {

EditorApplication::EditorApplication(const String& projectFilePath)
	: Application(projectFilePath)
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::onStartImpl()
{
}

void EditorApplication::onUpdateImpl(f32 deltaTime)
{
}

void EditorApplication::onShutdownImpl()
{
}

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING