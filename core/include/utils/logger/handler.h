#pragma once
#include "platforms/common.h"
#include "utils/logger/types.h"

namespace ntt {

/**
 * The interface for different log handlers (e.g., console, file, network).
 */
class Handler
{
public:
	virtual ~Handler() = default;

	/**
	 * Handle a log message with a specific severity level.
	 *
	 * @param record The log record containing all information about the log message.
	 */
	virtual void Process(const LogRecord& record) = 0;
};

} // namespace ntt
