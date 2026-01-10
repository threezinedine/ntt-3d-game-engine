#pragma once
#include "platforms/common.h"

#define LOGGER_BUFFER_SIZE 4096

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
	LOG_LEVEL_COUNT NTT_HIDDEN
};

/**
 * All bit masks for hiding/showing log messages with specific tags.
 */
enum NTT_BINDING LogTagMaskBit : u32
{
	LOG_TAG_MASK_SYSTEM	  = NTT_BIT(0), /// All utilities/core related messages
	LOG_TAG_MASK_RESOURCE = NTT_BIT(1), /// Resource management related messages
	// Add more tags here
	// Add more tags here
	LOG_TAG_MASK_ALL	  = ~0u
};

/**
 * The whole information of the message.
 */
struct LogRecord
{
	LogLevel	  level;
	char		  message[LOGGER_BUFFER_SIZE];
	const char*	  file;
	int			  line;
	LogTagMaskBit tag;
};

enum NTT_BINDING LogHandlerType : u8
{
	LOG_HANDLER_TYPE_CONSOLE = NTT_BIT(0),
	LOG_HANDLER_TYPE_FILE	 = NTT_BIT(1),
	LOG_HANDLER_TYPE_NETWORK = NTT_BIT(2),
	LOG_HANDLER_TYPE_COUNT NTT_HIDDEN
};

typedef u8 LogHandlerTypes;

} // namespace ntt
