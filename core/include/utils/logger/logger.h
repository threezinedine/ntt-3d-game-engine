#pragma once
#include "handler.h"
#include "platforms/common.h"
#include "types.h"

#define LOGGER_BUFFER_SIZE 4096

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
	 */
	void Setup(LogLevel level, const char* format, LogHandlerTypes types) NTT_BINDING;

	/**
	 * This method is called for all the handlers to process the log message.
	 * @param level The log level to set.
	 * @param message The log message to handle.
	 * @param file The source file where the log message originated.
	 * @param line The line number in the source file where the log message originated.
	 */
	void Log(LogLevel level, const char* message, const char* file, u8 line) NTT_BINDING;

private:
	Array<Scope<Handler>> m_handlers;
	LogLevel			  m_logLevel;
	const char*			  m_format;
};

} // namespace ntt

#if NTT_DEBUG
#define NTT_LOG_TRACE(message, ...)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_TRACE, buffer, __FILE__, __LINE__);                                       \
	} while (0)

#define NTT_LOG_DEBUG(message, ...)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_DEBUG, buffer, __FILE__, __LINE__);                                       \
	} while (0)

#else
#define NTT_LOG_TRACE(message, ...)
#define NTT_LOG_DEBUG(message, ...)
#endif

#define NTT_LOG_INFO(message, ...)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_INFO, buffer, __FILE__, __LINE__);                                        \
	} while (0)

#define NTT_LOG_WARN(message, ...)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_WARN, buffer, __FILE__, __LINE__);                                        \
	} while (0)

#define NTT_LOG_ERROR(message, ...)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_ERROR, buffer, __FILE__, __LINE__);                                       \
	} while (0)

#define NTT_LOG_FATAL(message, ...)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[LOGGER_BUFFER_SIZE];                                                                               \
		std::snprintf(buffer, LOGGER_BUFFER_SIZE, message, ##__VA_ARGS__);                                             \
		Logger::GetInstance()->Log(LOG_LEVEL_FATAL, buffer, __FILE__, __LINE__);                                       \
	} while (0)
