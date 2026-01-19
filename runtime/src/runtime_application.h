#pragma once
#include <engine/engine.h>

namespace ntt {

class Window;
class Program;

class RuntimeApplication : public Application
{
public:
	RuntimeApplication();
	virtual ~RuntimeApplication();

protected:
	virtual void startBeginImpl() override;
	virtual void startEndImpl() override;

	virtual void updateBeginImpl(f32 deltaTime) override;
	virtual void updateImpl(f32 deltaTime) override;
	virtual void updateEndImpl(f32 deltaTime) override;

	virtual void shutdownBeginImpl() override;
	virtual void shutdownEndImpl() override;

private:
	Scope<Program>		 m_pProgram;
	Scope<VertexBuffer>	 m_pVertexBuffer;
	Scope<UniformBuffer> m_pUniformBuffer;
};

} // namespace ntt