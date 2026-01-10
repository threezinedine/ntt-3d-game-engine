#pragma once

#include "platforms/common.h"
#include "utils/logger/handler.h"

namespace ntt {
class EditorLogHandler : public Handler
{
public:
	EditorLogHandler(LogEditorCallbackFn callback);
	virtual ~EditorLogHandler() override;
	virtual void Process(const LogRecord& record) override;

private:
	LogEditorCallbackFn m_callback;
};

} // namespace ntt
