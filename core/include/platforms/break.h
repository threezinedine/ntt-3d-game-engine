#pragma once

namespace ntt {

/**
 * Used for trigger the error breakpoint in the debugger or halt the program execution.
 * This is useful for debugging purposes, especially when an assertion fails.
 * On supported platforms, this will trigger a breakpoint interrupt.
 */
void debugBreak();

} // namespace ntt
