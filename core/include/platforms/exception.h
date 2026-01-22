#pragma once

#include "pch.h"

namespace ntt {

/**
 * Possible signals for interrupting the normal flow of program execution.
 */
enum ExceptionType
{
	EXCEPTION_TYPE_UNKNOWN,
	EXCEPTION_TYPE_RUNTIME,
	EXCEPTION_TYPE_LOGIC,
	EXCEPTION_TYPE_OUT_OF_MEMORY,
	EXCEPTION_TYPE_INVALID_ARGUMENT,
	EXCEPTION_TYPE_INDEX_OUT_OF_BOUNDS,
	EXCEPTION_TYPE_FILE_NOT_FOUND,
	EXCEPTION_TYPE_IO_ERROR,
	EXCEPTION_TYPE_COUNT,
};

/**
 * Convert the exception type enum to a human-readable string.
 *
 * @param type The exception type to convert.
 * @return The string representation of the exception type.
 */
const char* exceptionTypeToString(ExceptionType type);

/**
 * Main interface for triggering exceptions in the engine.
 *
 * @param type The type of exception to raise.
 * @param message The message associated with the exception.
 */
void raiseException(ExceptionType type, const char* message);

} // namespace ntt
