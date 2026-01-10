#pragma once

#include "platforms/common.h"
#include "utils/logger/handler.h"

namespace ntt {
class EditorLogHandler : public Handler
{
public:
	EditorLogHandler();
	virtual ~EditorLogHandler() override;
	virtual void Process(const LogRecord& record) override;
};

} // namespace ntt
