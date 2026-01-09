#include "platforms/console.h"
#include <cstdarg>
#include <cstdio>

namespace ntt {
ConsoleColor Console::s_currentColor = CONSOLE_COLOR_DEFAULT;

void Console::setColor(ConsoleColor color)
{
	s_currentColor = color;
	switch (s_currentColor)
	{
	case CONSOLE_COLOR_BLACK:
		std::printf("\033[30m");
		break;
	case CONSOLE_COLOR_RED:
		std::printf("\033[31m");
		break;
	case CONSOLE_COLOR_GREEN:
		std::printf("\033[32m");
		break;
	case CONSOLE_COLOR_YELLOW:
		std::printf("\033[33m");
		break;
	case CONSOLE_COLOR_BLUE:
		std::printf("\033[34m");
		break;
	case CONSOLE_COLOR_MAGENTA:
		std::printf("\033[35m");
		break;
	case CONSOLE_COLOR_CYAN:
		std::printf("\033[36m");
		break;
	case CONSOLE_COLOR_WHITE:
		std::printf("\033[37m");
		break;
	case CONSOLE_COLOR_DEFAULT:
		std::printf("\033[39m");
		break;

	default:
		NTT_UNREACHABLE();
	}
}

void Console::resetColor()
{
	s_currentColor = CONSOLE_COLOR_WHITE;
	std::printf("\033[37m");
}

void Console::print(const char* message)
{
	// Note: Actual color changing code would go here, platform-specific.
	// For simplicity, we just print the message.
	std::printf("%s\n", message);
}

void Console::printf(const char* format, ...)
{
	// Note: Actual color changing code would go here, platform-specific.
	// For simplicity, we just print the formatted message.
	va_list args;
	va_start(args, format);
	std::vprintf(format, args);
	std::printf("\n");
	va_end(args);
}

} // namespace ntt
