#include <engine/engine.h>

using namespace ntt;

int main()
{
	console::print("Hello, NTT 3D Game Engine!");
	console::print("Hello, NTT 3D Game Engine!", console::CONSOLE_COLOR_DEFAULT, console::CONSOLE_COLOR_BLUE);
	console::print("Hello, NTT 3D Game Engine!", console::CONSOLE_COLOR_GREEN);
	console::print("Hello, NTT 3D Game Engine!", console::CONSOLE_COLOR_RED, console::CONSOLE_COLOR_WHITE);
	console::print("Hello, NTT 3D Game Engine!", console::CONSOLE_COLOR_YELLOW, console::CONSOLE_COLOR_DEFAULT, true);
	return 0;
}