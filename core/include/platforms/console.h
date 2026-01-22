#pragma once
#include "pch.h"

namespace ntt {

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

/**
 * Print a message to the console with optional text and background colors, and bold formatting.
 *
 * @param message The message to print to the console.
 * @param textColor The color of the text. Default is CONSOLE_COLOR_DEFAULT.
 * @param backgroundColor The color of the background. Default is CONSOLE_COLOR_DEFAULT.
 * @param isBold Whether the text should be printed in bold. Default is false.
 */
void print(const char*	message,
		   ConsoleColor textColor		= CONSOLE_COLOR_DEFAULT,
		   ConsoleColor backgroundColor = CONSOLE_COLOR_DEFAULT,
		   b8			isBold			= false);

} // namespace ntt
