#include "utils/logger/logger.h"
#include "console_handler.h"

namespace ntt {
NTT_SINGLETON_DEFINE(Logger)

Logger::Logger()
	: m_logLevel(LOG_LEVEL_INFO)
{
}

Logger::~Logger()
{
}

void Logger::Setup(LogLevel level, const char* format, LogHandlerTypes types)
{
	m_logLevel = level;

	if (types & LOG_HANDLER_TYPE_CONSOLE)
	{
		m_handlers.push_back(CreateScope<ConsoleHandler>());
	}

	m_format = format;
}

void Logger::Log(LogLevel level, const char* message, const char* file, u8 line)
{
	if (level < m_logLevel)
	{
		return;
	}

	LogRecord record;
	record.level   = level;
	record.message = message;
	record.file	   = file;
	record.line	   = line;

	// Handle message here

	for (auto& handler : m_handlers)
	{
		handler->Process(record);
	}
}

} // namespace ntt
