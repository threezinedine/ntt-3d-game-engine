#include "editor_handler.h"
#include "utils/logger/logger.h"

namespace ntt {
EditorLogHandler::EditorLogHandler()
{
}

EditorLogHandler::~EditorLogHandler()
{
}

void EditorLogHandler::Process(const LogRecord& record)
{
	if (Logger::GetInstance()->GetEditorCallback())
	{
		Logger::GetInstance()->GetEditorCallback()(record);
	}
}

} // namespace ntt
