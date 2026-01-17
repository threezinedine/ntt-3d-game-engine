#include "project/layer.h"
#include "project/application.h"

namespace ntt {

Layer::Layer()
	: m_pApplication(nullptr)
{
}

Layer::Layer(Layer&& other)
	: m_pApplication(other.m_pApplication)
{
	other.m_pApplication = nullptr;
}

Layer::~Layer()
{
}

void Layer::OnAttach(Application* pApplication)
{
	m_pApplication = pApplication;
	attachImpl();
}

void Layer::OnUpdate(f32 deltaTime)
{
	updateImpl(deltaTime);
}

void Layer::OnDetach()
{
	detachImpl();
	m_pApplication = nullptr;
}

} // namespace ntt