#include <engine/engine.h>

using namespace ntt;

int main()
{
	Logger::GetInstance()->Setup(
		LOG_LEVEL_DEBUG, "[@5:name] - [@7:level] - @:message", LOG_HANDLER_TYPE_CONSOLE, LOG_TAG_MASK_ALL);

	NTT_SYSTEM_LOG_DEBUG("Starting initialization sequence");
	NTT_SYSTEM_LOG_INFO("Loading configuration files");

	InitializeWindowingSystem();

	Window window("NTT Engine Window", 1280, 720);

	window.Initialize();

	while (window.IsOpen())
	{
		window.PollEvents();
	}

	NTT_SYSTEM_LOG_INFO("Shutting down application");
	window.Shutdown();

	ShutdownWindowingSystem();

	return 0;
}