#include "editor_handler.h"

namespace ntt {
EditorLogHandler::EditorLogHandler(LogEditorCallbackFn callback)
	: m_callback(callback)
{
}

EditorLogHandler::~EditorLogHandler()
{
}

void EditorLogHandler::Process(const LogRecord& record)
{
	if (m_callback)
	{
		m_callback(record);
	}
}

} // namespace ntt
