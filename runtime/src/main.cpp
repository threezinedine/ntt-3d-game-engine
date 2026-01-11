#include <engine/engine.h>

using namespace ntt;

int main()
{
	Logger::GetInstance()->Setup(
		LOG_LEVEL_DEBUG, "[@5:name] - [@7:level] - @:message", LOG_HANDLER_TYPE_CONSOLE, LOG_TAG_MASK_ALL);

	Scope<Application> app = CreateScope<RuntimeApplication>("path/to/project.nttproj");

	app->Start();

	while (app->IsOpen())
	{
		app->Update(0.016f); // Assuming a fixed delta time for simplicity
	}

	app->Shutdown();

	return 0;
}