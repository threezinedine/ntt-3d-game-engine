#include "utils/logger/handler.h"

namespace ntt {
class ConsoleHandler : public Handler
{
public:
	void Process(const LogRecord& record) override;
};
} // namespace ntt
