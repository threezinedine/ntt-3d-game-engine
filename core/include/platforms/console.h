#pragma once
#include "pch.h"

namespace ntt::console {

/**
 * All possible color which the console can use to print text and the
 *      background. This enum is available for all supported platforms.
 */
enum ConsoleColor : u8
{
	CONSOLE_COLOR_DEFAULT = 0,
	CONSOLE_COLOR_BLACK,
	CONSOLE_COLOR_RED,
	CONSOLE_COLOR_GREEN,
	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_BLUE,
	CONSOLE_COLOR_MAGENTA,
	CONSOLE_COLOR_CYAN,
	CONSOLE_COLOR_WHITE,
	CONSOLE_COLOR_COUNT,
};

void print(const char*	message,
		   ConsoleColor textColor		= CONSOLE_COLOR_DEFAULT,
		   ConsoleColor backgroundColor = CONSOLE_COLOR_DEFAULT,
		   b8			isBold			= false);

} // namespace ntt::console
