#if NTT_IDE_APPLICATION
#pragma once
#include "application.h"
#include "platforms/common.h"

namespace ntt {

class IDEApplication : public Application
{
public:
	IDEApplication();
	virtual ~IDEApplication();

protected:
	virtual void startBeginImpl() override;
	virtual void startEndImpl() override;
	virtual void updateBeginImpl(f32 deltaTime) override;
	virtual void updateImpl(f32 deltaTime) override;
	virtual void updateEndImpl(f32 deltaTime) override;
	virtual void shutdownBeginImpl() override;
	virtual void shutdownEndImpl() override;
};

} // namespace ntt

#endif // NTT_IDE_APPLICATION