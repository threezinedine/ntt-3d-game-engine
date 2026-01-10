#if NTT_USE_GLFW

#include "platforms/graphics/window.h"
#include <GLFW/glfw3.h>

namespace ntt {
class Window::WindowInternal
{
public:
	GLFWwindow* m_window;
};

Window::Window(const char* title, u32 width, u32 height)
	: m_width(width)
	, m_height(height)
	, m_title(title)
	, m_isOpen(NTT_FALSE)
	, m_pSurface(nullptr)
{
	m_internal			 = CreateScope<WindowInternal>();
	m_internal->m_window = nullptr;
}

Window::~Window()
{
}

void Window::Initialize()
{
	m_isOpen			 = NTT_TRUE;
	m_internal->m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	m_pSurface			 = CreateRef<Surface>(m_internal->m_window);
	m_pSurface->Bind();
}

void Window::Shutdown()
{
	m_isOpen = NTT_FALSE;
	m_pSurface.reset();
	glfwDestroyWindow(m_internal->m_window);
}

void Window::PollEvents()
{
	NTT_ASSERT(m_pSurface != nullptr);
	m_isOpen = !glfwWindowShouldClose(m_pSurface->GetGLFWWindow());
	glfwPollEvents();
}

void InitializeWindowingSystem()
{
	NTT_ASSERT(glfwInit());
}

void ShutdownWindowingSystem()
{
	glfwTerminate();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL