#pragma once
#include "handler.h"
#include "platforms/common.h"
#include "types.h"

namespace ntt {

/**
 * The singleton module which working as the central logger for the engine.
 */
class NTT_BINDING NTT_SINGLETON Logger
{
	NTT_SINGLETON_DECLARE(Logger)

public:
	/**
	 * Configure the logger with specified log level, format, and handler types.
	 *
	 * @param level The log level to set.
	 * @param format The format string for the log message. For example: "[@5:name] - [@7:level] - @:message"
	 * Then the output will be like: "[MYAPP] - [   INFO] - Initialization complete"
	 * @param types The types of log handlers to use (e.g., console, file, network).
	 * @param tagMask The tag mask to filter log messages.
	 */
	void Setup(LogLevel level, const char* format, LogHandlerTypes types, u32 tagMask) NTT_BINDING;

	/**
	 * This method is called for all the handlers to process the log message.
	 * @param level The log level to set.
	 * @param tag The tag mask associated with the log message.
	 * @param message The log message to handle.
	 * @param file The source file where the log message originated.
	 * @param line The line number in the source file where the log message originated.
	 */
	void Log(LogLevel level, LogTagMaskBit tag, const char* message, const char* file, u8 line) NTT_BINDING;

	/**
	 * The interface whereas the editor can be notified of log messages from the engine.
	 * @param callback The callback function to be called for each log message.
	 */
	inline void SetupEditorCallback(LogEditorCallbackFn callback) NTT_BINDING;

	inline LogEditorCallbackFn GetEditorCallback() const
	{
		return m_editorCallback;
	}

private:
	Array<Scope<Handler>> m_handlers;
	LogLevel			  m_logLevel;
	u32					  m_tagMask;
	const char*			  m_format;
	LogEditorCallbackFn	  m_editorCallback;
};

inline void Logger::SetupEditorCallback(LogEditorCallbackFn callback)
{
	m_editorCallback = callback;
}

} // namespace ntt

#if NTT_DEBUG
#define NTT_LOG_TRACE(tag, message, ...)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_TRACE, tag, buffer, __FILE__, __LINE__);                                  \
	} while (0)

#define NTT_LOG_DEBUG(tag, message, ...)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_DEBUG, tag, buffer, __FILE__, __LINE__);                                  \
	} while (0)

#else
#define NTT_LOG_TRACE(tag, message, ...)
#define NTT_LOG_DEBUG(tag, message, ...)
#endif

#define NTT_LOG_INFO(tag, message, ...)                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_INFO, tag, buffer, __FILE__, __LINE__);                                   \
	} while (0)

#define NTT_LOG_WARN(tag, message, ...)                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_WARN, tag, buffer, __FILE__, __LINE__);                                   \
	} while (0)

#define NTT_LOG_ERROR(tag, message, ...)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_ERROR, tag, buffer, __FILE__, __LINE__);                                  \
	} while (0)

#define NTT_LOG_FATAL(tag, message, ...)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_FATAL, tag, buffer, __FILE__, __LINE__);                                  \
	} while (0)

#define NTT_SYSTEM_LOG_TRACE(message, ...) NTT_LOG_TRACE(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
#define NTT_SYSTEM_LOG_DEBUG(message, ...) NTT_LOG_DEBUG(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
#define NTT_SYSTEM_LOG_INFO(message, ...)  NTT_LOG_INFO(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
#define NTT_SYSTEM_LOG_WARN(message, ...)  NTT_LOG_WARN(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
#define NTT_SYSTEM_LOG_ERROR(message, ...) NTT_LOG_ERROR(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
#define NTT_SYSTEM_LOG_FATAL(message, ...) NTT_LOG_FATAL(LOG_TAG_MASK_SYSTEM, message, ##__VA_ARGS__)
