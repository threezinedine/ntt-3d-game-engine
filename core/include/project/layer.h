#pragma once
#include "platforms/common.h"

namespace ntt {

class Application;

class Layer
{
public:
	Layer();
	NTT_DELETE_COPY(Layer)
	Layer(Layer&& other);
	virtual ~Layer();

public:
	void OnAttach(Application* pApplication);
	void OnUpdate(f32 deltaTime);
	void OnDetach();

public:
	inline b8 IsAttached() const
	{
		return m_pApplication != nullptr;
	}

protected:
	virtual void attachImpl() {};
	virtual void updateImpl(f32 deltaTime) {};
	virtual void detachImpl() {};

private:
	Application* m_pApplication;
};

} // namespace ntt
