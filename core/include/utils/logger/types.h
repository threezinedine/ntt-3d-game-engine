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
	LOG_TAG_MASK_SYSTEM		 = NTT_BIT(0), /// All utilities/core related messages
	LOG_TAG_MASK_RESOURCE	 = NTT_BIT(1), /// Resource management related messages
	LOG_TAG_MASK_APPLICATION = NTT_BIT(2), /// Application level messages
	LOG_TAG_MASK_RENDERER	 = NTT_BIT(3), /// Renderer related messages
	LOG_TAG_MASK_VULKAN		 = NTT_BIT(4),
	LOG_TAG_MASK_OPENGL		 = NTT_BIT(5),
	// Add more tags here
	// Add more tags here
	LOG_TAG_MASK_ALL		 = ~0u
};

/**
 * The whole information of the message.
 */
struct NTT_BINDING LogRecord
{
	LogLevel	  level;
	String		  message;
	String		  file;
	u32			  line;
	LogTagMaskBit tag;
	String		  fullFilePath;
	String		  finalMessage;
};

enum NTT_BINDING LogHandlerType : u8
{
	LOG_HANDLER_TYPE_CONSOLE = NTT_BIT(0),
	LOG_HANDLER_TYPE_FILE	 = NTT_BIT(1),
	LOG_HANDLER_TYPE_NETWORK = NTT_BIT(2),
	LOG_HANDLER_TYPE_EDITOR	 = NTT_BIT(3),
	LOG_HANDLER_TYPE_COUNT NTT_HIDDEN
};

const char* NTT_BINDING convertLoggerLevelToString(LogLevel level);
const char* NTT_BINDING convertLoggerTagToString(LogTagMaskBit tag);

typedef NTT_BINDING u8 LogHandlerTypes;

using LogEditorCallbackFn = std::function<void(const LogRecord& record)>;

} // namespace ntt
