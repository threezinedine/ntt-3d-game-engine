#pragma once

namespace ntt {

enum ConsoleColor
{
	BLACK = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	DEFAULT
};

/**
 * Unified console interface for different platforms.
 */
class Console
{
public:
	/**
	 * Modify the characters' color displayed on the console for subsequent print calls.
	 *
	 * @param color The color to set the console text to.
	 */
	static void setColor(ConsoleColor color);

	/**
	 * Recover the `WHITE` color for the console text.
	 */
	static void resetColor();

	/**
	 * Display a sentence on the console.
	 */
	static void print(const char* message);

private:
	static ConsoleColor s_currentColor;
};

} // namespace ntt
