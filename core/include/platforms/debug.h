#pragma once

#include "pch.h"

namespace ntt {

/**
 * Used for trigger the error breakpoint in the debugger or halt the program execution.
 * This is useful for debugging purposes, especially when an assertion fails.
 * On supported platforms, this will trigger a breakpoint interrupt.
 *
 * @note In release builds, this function may not have any effect.
 */
void debugBreak();

struct BacktraceInfo; ///< Forward for declaring the backtrace information of all supported platforms

/**
 * Retrieve the current backtrace information up to the specified maximum depth.
 *
 * @return The backtrace information containing the call stack addresses and count.
 *
 * @note In release builds, this function may not have any effect and return nullptr.
 */
BacktraceInfo* GetBacktrace();

/**
 * Print the backtrace information to the console for debugging purposes.
 *
 * @param pBacktraceInfo The backtrace information to print.
 */
void PrintBacktrace(BacktraceInfo* pBacktraceInfo);

} // namespace ntt
