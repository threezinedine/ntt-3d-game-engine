#include <engine/engine.h>

using namespace ntt;

int main()
{
	Logger::GetInstance()->Setup(LOG_LEVEL_INFO, "[@5:name] - [@7:level] - @:message", LOG_HANDLER_TYPE_CONSOLE);
	Logger::GetInstance()->Log(LOG_LEVEL_INFO, "Initialization complete", __FILE__, __LINE__);

	return 0;
}