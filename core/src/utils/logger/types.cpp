#include "utils/logger/logger.h"

namespace ntt {
const char* convertLoggerLevelToString(LogLevel level)
{
	switch (level)
	{
	case LOG_LEVEL_TRACE:
		return "TRACE";
	case LOG_LEVEL_DEBUG:
		return "DEBUG";
	case LOG_LEVEL_INFO:
		return "INFO";
	case LOG_LEVEL_WARN:
		return "WARNING";
	case LOG_LEVEL_ERROR:
		return "ERROR";
	case LOG_LEVEL_FATAL:
		return "FATAL";

	case LOG_LEVEL_COUNT:
		break;
	}

	NTT_UNREACHABLE();
	return "";
}

const char* convertLoggerTagToString(LogTagMaskBit tag)
{
	switch (tag)
	{
	case LOG_TAG_MASK_SYSTEM:
		return "SYSTEM";
	case LOG_TAG_MASK_RESOURCE:
		return "RESOURCE";
	case LOG_TAG_MASK_APPLICATION:
		return "APP";
	case LOG_TAG_MASK_RENDERER:
		return "RENDER";
	case LOG_TAG_MASK_VULKAN:
		return "VULKAN";
	case LOG_TAG_MASK_OPENGL:
		return "OPENGL";
		// Add more tags here
		// Add more tags here

	case LOG_TAG_MASK_ALL:
		return "ALL";
	}

	NTT_UNREACHABLE();
	return "UNKNOWN";
}

} // namespace ntt
