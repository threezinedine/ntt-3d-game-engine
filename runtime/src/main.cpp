#include <engine/engine.h>

using namespace ntt;

int main()
{
	MemorySystem::Initialize();

	{
		print("Hello, NTT 3D Game Engine!");
		print("Hello, NTT 3D Game Engine!", CONSOLE_COLOR_DEFAULT, CONSOLE_COLOR_BLUE);
		print("Hello, NTT 3D Game Engine!", CONSOLE_COLOR_GREEN);
		print("Hello, NTT 3D Game Engine!", CONSOLE_COLOR_RED, CONSOLE_COLOR_WHITE);
		print("Hello, NTT 3D Game Engine!", CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT, true);

		String helloStr("Hello, NTT String!");

		print(helloStr.c_str(), CONSOLE_COLOR_CYAN);
	}

	MemorySystem::Shutdown();
	return 0;
}