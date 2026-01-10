#include "console_handler.h"

namespace ntt {

void ConsoleHandler::Process(const LogRecord& record)
{
	switch (record.level)
	{
	case LOG_LEVEL_TRACE:
		ntt::Console::setColor(CONSOLE_COLOR_CYAN);
		break;
	case LOG_LEVEL_DEBUG:
		ntt::Console::setColor(CONSOLE_COLOR_BLUE);
		break;
	case LOG_LEVEL_INFO:
		ntt::Console::setColor(CONSOLE_COLOR_GREEN);
		break;
	case LOG_LEVEL_WARN:
		ntt::Console::setColor(CONSOLE_COLOR_YELLOW);
		break;
	case LOG_LEVEL_ERROR:
		ntt::Console::setColor(CONSOLE_COLOR_RED);
		break;
	case LOG_LEVEL_FATAL:
		ntt::Console::setColor(CONSOLE_COLOR_MAGENTA);
		break;

	case LOG_LEVEL_COUNT:
		NTT_UNREACHABLE();
	}

	ntt::Console::printf("%s", record.message.c_str());
}

} // namespace ntt
