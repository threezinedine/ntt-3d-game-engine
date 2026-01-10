#if NTT_USE_GLFW

#include "platforms/window.h"
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
{
	m_internal			 = CreateScope<WindowInternal>();
	m_internal->m_window = nullptr;
}

Window::~Window()
{
}

void Window::Initialize()
{
	// Platform-specific window creation code goes here.
	m_isOpen			 = NTT_TRUE;
	m_internal->m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_internal->m_window);
}

void Window::Shutdown()
{
	// Platform-specific window destruction code goes here.
	m_isOpen = NTT_FALSE;
	glfwDestroyWindow(m_internal->m_window);
}

void Window::PollEvents()
{
	// Platform-specific event polling code goes here.
	m_isOpen = !glfwWindowShouldClose(glfwGetCurrentContext());
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