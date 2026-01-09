#pragma once
#include "platforms/common.h"

namespace ntt {
/**
 * List of serverity for the message to log.
 */
enum NTT_BINDING LogLevel
{
	LOG_LEVEL_TRACE, /// Detailed information, typically of interest only when diagnosing problems.
	LOG_LEVEL_DEBUG, /// Information useful to developers for debugging the application.
	LOG_LEVEL_INFO,	 /// Informational messages that highlight the progress of the application at coarse-grained level.
	LOG_LEVEL_WARN,	 /// Potentially harmful situations which still allow the application to continue running.
	LOG_LEVEL_ERROR, /// Error events that might still allow the application to continue running.
	LOG_LEVEL_FATAL, /// Very severe error events that will presumably lead the application to abort.
	COUNT NTT_HIDDEN
};

/**
 * The whole information of the message.
 */
struct LogRecord
{
	LogLevel	level;
	const char* message;
	const char* file;
	int			line;
};

enum NTT_BINDING LogHandlerType : u8
{
	LOG_HANDLER_TYPE_CONSOLE = 0x01,
	LOG_HANDLER_TYPE_FILE	 = 0x01 << 1,
	LOG_HANDLER_TYPE_NETWORK = 0x01 << 2,
	LOG_HANDLER_TYPE_COUNT NTT_HIDDEN
};

typedef u8 LogHandlerTypes;

} // namespace ntt
