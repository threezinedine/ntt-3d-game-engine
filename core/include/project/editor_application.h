#if NTT_ENGINE_EDITOR_BINDING || !NTT_NO_ANALYZE
#pragma once
#include "application.h"
#include "platforms/common.h"

namespace ntt {

class NTT_BINDING EditorApplication : public Application
{
public:
	EditorApplication(const String& projectFilePath);
	virtual ~EditorApplication();

protected:
	virtual void onStartImpl() override;
	virtual void onUpdateImpl(f32 deltaTime) override;
	virtual void onShutdownImpl() override;
};

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING