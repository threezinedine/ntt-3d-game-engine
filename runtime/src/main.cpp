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

		Path testPath("/tmp/test.txt");
		print(testPath.isExisted() ? "File exists" : "File does not exist");

		Path testDir("/tmp");
		print(testDir.isDirectory() ? "It's a directory" : "It's not a directory");

		Path testExitingFile("/etc/passwd");
		print(testExitingFile.isFile() ? "It's a file" : "It's not a file");

		print(helloStr.c_str(), CONSOLE_COLOR_CYAN);

		int* testInt = (int*)MemorySystem::getDefaultAllocator()->allocate(sizeof(int), alignof(int));
		*testInt	 = 42;
		MemorySystem::getDefaultAllocator()->deallocate(testInt, sizeof(int));
	}

	MemorySystem::Shutdown();
	return 0;
}