#include "utils/logger/logger.h"
#include "console_handler.h"
#include "editor_handler.h"
#include <filesystem>

#define LOG_LEVEL_TAG_STRING_MAX_LENGTH 10
#define LOG_LEVEL_STRING_MAX_LENGTH		7
#define LOG_FILENAME_MAX_LENGTH			20

namespace ntt {
NTT_SINGLETON_DEFINE(Logger)

Logger::Logger()
	: m_logLevel(LOG_LEVEL_INFO)
	, m_tagMask(LOG_TAG_MASK_ALL)
	, m_format(nullptr)
	, m_editorCallback(nullptr)
{
}

Logger::~Logger()
{
}

void Logger::Setup(LogLevel level, const char* format, LogHandlerTypes types, u32 tagMask)
{
	m_logLevel = level;
	m_tagMask  = tagMask;

	if (types & LOG_HANDLER_TYPE_CONSOLE)
	{
		m_handlers.push_back(CreateScope<ConsoleHandler>());
	}

	if (types & LOG_HANDLER_TYPE_EDITOR)
	{
		m_handlers.push_back(CreateScope<EditorLogHandler>());
	}

	m_format = format;
}

static void truncateString(const String& input, char* output, size_t maxLength);

void Logger::Log(LogLevel level, LogTagMaskBit tag, const char* message, const char* file, u32 line)
{
	if (level < m_logLevel)
	{
		return;
	}

	LogRecord record;
	record.level		= level;
	record.message		= String(message);
	record.fullFilePath = file;
	record.line			= line;
	record.tag			= tag;

	if (!(tag & m_tagMask))
	{
		return;
	}

	const char* levelStr = convertLoggerLevelToString(level);
	const char* tagStr	 = convertLoggerTagToString(tag);

	String filename = std::filesystem::path(record.fullFilePath).filename().string();
	char   finalFilename[LOG_FILENAME_MAX_LENGTH + 1];
	truncateString(filename, finalFilename, LOG_FILENAME_MAX_LENGTH);

	char messageBuffer[LOGGER_BUFFER_SIZE];
	std::memset(messageBuffer, 0, LOGGER_BUFFER_SIZE);
	std::sprintf(messageBuffer, "[%7s] - [%7s] - %20s:%-4d - %s", tagStr, levelStr, finalFilename, line, message);
	record.finalMessage = String(messageBuffer);
	record.file			= String(finalFilename);

	for (auto& handler : m_handlers)
	{
		handler->Process(record);
	}
}

static void truncateString(const String& input, char* output, size_t maxLength)
{
	std::string etc		  = "...";
	u32			etcLength = static_cast<u32>(etc.length());

	std::memset(output, 0, maxLength + 1);

	if (input.length() <= maxLength)
	{
		std::sprintf(output, "%s", input.c_str());
	}
	else
	{
		std::sprintf(output, "%.*s%s", static_cast<int>(maxLength - etcLength), input.c_str(), etc.c_str());
	}
}

} // namespace ntt
