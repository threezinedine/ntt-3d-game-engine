#include <engine/engine.h>

using namespace ntt;

int main()
{
	Logger::GetInstance()->Setup(LOG_LEVEL_INFO, "[@5:name] - [@7:level] - @:message", LOG_HANDLER_TYPE_CONSOLE);

	NTT_LOG_DEBUG("Debugging the application");
	NTT_LOG_INFO("Initialization complete");

	return 0;
}