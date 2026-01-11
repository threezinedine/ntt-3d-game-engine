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

void EditorApplication::startBeginImpl()
{
}

void EditorApplication::startEndImpl()
{
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
}

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING