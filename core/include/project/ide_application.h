#if NTT_IDE_APPLICATION

#include "application.h"
#include "platforms/common.h"

namespace ntt {

class Window;

class IDEApplication : public Application
{
public:
	IDEApplication();
	virtual ~IDEApplication();

public:
	b8 IsOpen() const override;

protected:
	virtual void startBeginImpl() override;
	virtual void startEndImpl() override;
	virtual void updateBeginImpl(f32 deltaTime) override;
	virtual void updateEndImpl(f32 deltaTime) override;
	virtual void shutdownBeginImpl() override;
	virtual void shutdownEndImpl() override;

private:
	Scope<Window> m_pWindow;
};

} // namespace ntt

#endif // NTT_IDE_APPLICATION