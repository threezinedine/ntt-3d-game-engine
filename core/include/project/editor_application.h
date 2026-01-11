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

	inline b8 IsOpen() const override
	{
		return true;
	}

protected:
	virtual void startBeginImpl() override;
	virtual void startEndImpl() override;
	virtual void updateBeginImpl(f32 deltaTime) override;
	virtual void updateEndImpl(f32 deltaTime) override;
	virtual void shutdownBeginImpl() override;
	virtual void shutdownEndImpl() override;
};

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING