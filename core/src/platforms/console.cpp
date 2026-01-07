#include "platforms/console.h"
#include <cstdio>

namespace ntt {
ConsoleColor Console::s_currentColor = ConsoleColor::DEFAULT;

void Console::setColor(ConsoleColor color)
{
	s_currentColor = color;
}

void Console::resetColor()
{
	s_currentColor = ConsoleColor::WHITE;
}

void Console::print(const char* message)
{
	// Note: Actual color changing code would go here, platform-specific.
	// For simplicity, we just print the message.
	std::printf("%s\n", message);
}

} // namespace ntt
