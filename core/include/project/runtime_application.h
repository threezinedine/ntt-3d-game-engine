#if !NTT_ENGINE_EDITOR_BINDING && NTT_NO_ANALYZE
#pragma once
#include "application.h"
#include "platforms/common.h"

namespace ntt {

class Window;
class Program;

class RuntimeApplication : public Application
{
public:
	RuntimeApplication();
	virtual ~RuntimeApplication();

	virtual b8 IsOpen() const override;

protected:
	virtual void startBeginImpl() override;
	virtual void startEndImpl() override;

	virtual void updateBeginImpl(f32 deltaTime) override;
	virtual void updateEndImpl(f32 deltaTime) override;

	virtual void shutdownBeginImpl() override;
	virtual void shutdownEndImpl() override;

private:
	Scope<Program>	  m_pProgram;
	Reference<Window> m_pWindow;
};

} // namespace ntt

#endif // NTT_ENGINE_EDITOR_BINDING
