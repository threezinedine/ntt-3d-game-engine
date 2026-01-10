#include <engine/engine.h>

using namespace ntt;

int main()
{
	Logger::GetInstance()->Setup(
		LOG_LEVEL_DEBUG, "[@5:name] - [@7:level] - @:message", LOG_HANDLER_TYPE_CONSOLE, LOG_TAG_MASK_ALL);

	NTT_SYSTEM_LOG_DEBUG("Starting initialization sequence");
	NTT_SYSTEM_LOG_INFO("Loading configuration files");

	return 0;
}