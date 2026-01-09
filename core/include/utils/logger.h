#pragma once

namespace ntt {

/**
 * List of serverity for the message to log.
 */
enum LogLevel
{
	Trace, /// Detailed information, typically of interest only when diagnosing problems.
	Debug, /// Information useful to developers for debugging the application.
	Info,  /// Informational messages that highlight the progress of the application at coarse-grained level.
	Warn,  /// Potentially harmful situations which still allow the application to continue running.
	Error, /// Error events that might still allow the application to continue running.
	Fatal  /// Very severe error events that will presumably lead the application to abort.
};

/**
 * The singleton module which working as the central logger for the engine.
 */
class Logger
{
};

} // namespace ntt
