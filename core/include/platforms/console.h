#pragma once
#include "common.h"

namespace ntt {

enum NTT_BINDING ConsoleColor
{
	CONSOLE_COLOR_BLACK,
	CONSOLE_COLOR_RED,
	CONSOLE_COLOR_GREEN,
	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_BLUE,
	CONSOLE_COLOR_MAGENTA,
	CONSOLE_COLOR_CYAN,
	CONSOLE_COLOR_WHITE,
	CONSOLE_COLOR_DEFAULT,
	CONSOLE_COLOR_COUNT NTT_HIDDEN
};

/**
 * Unified console interface for different platforms.
 */
class NTT_BINDING NTT_SINGLETON Console
{
public:
	/**
	 * Modify the characters' color displayed on the console for subsequent print calls.
	 *
	 * @param color The color to set the console text to.
	 */
	static void setColor(ConsoleColor color) NTT_BINDING;

	/**
	 * Recover the `WHITE` color for the console text.
	 */
	static void resetColor() NTT_BINDING;

	/**
	 * Display a sentence on the console.
	 */
	static void print(const char* message) NTT_BINDING;

	/**
	 * Display a formatted sentence on the console.
	 */
	static void printf(const char* format, ...);

private:
	static ConsoleColor s_currentColor;
};

} // namespace ntt
