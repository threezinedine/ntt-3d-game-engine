#if NTT_USE_GLFW

#include "graphics/surface.h"
#include "input/input.h"
#include "platforms/graphics/window.h"
#include "utils/utils.h"
#include <GLFW/glfw3.h>

namespace ntt {

constexpr i32 GLFWKeyMaps[KEY_CODE_COUNT] = {
	[KEY_CODE_NONE] = GLFW_KEY_UNKNOWN,
	// Letters
	[KEY_CODE_A]	= GLFW_KEY_A,
	[KEY_CODE_B]	= GLFW_KEY_B,
	[KEY_CODE_C]	= GLFW_KEY_C,
	[KEY_CODE_D]	= GLFW_KEY_D,
	[KEY_CODE_E]	= GLFW_KEY_E,
	[KEY_CODE_F]	= GLFW_KEY_F,
	[KEY_CODE_G]	= GLFW_KEY_G,
	[KEY_CODE_H]	= GLFW_KEY_H,
	[KEY_CODE_I]	= GLFW_KEY_I,
	[KEY_CODE_J]	= GLFW_KEY_J,
	[KEY_CODE_K]	= GLFW_KEY_K,
	[KEY_CODE_L]	= GLFW_KEY_L,
	[KEY_CODE_M]	= GLFW_KEY_M,
	[KEY_CODE_N]	= GLFW_KEY_N,
	[KEY_CODE_O]	= GLFW_KEY_O,
	[KEY_CODE_P]	= GLFW_KEY_P,
	[KEY_CODE_Q]	= GLFW_KEY_Q,
	[KEY_CODE_R]	= GLFW_KEY_R,
	[KEY_CODE_S]	= GLFW_KEY_S,
	[KEY_CODE_T]	= GLFW_KEY_T,
	[KEY_CODE_U]	= GLFW_KEY_U,
	[KEY_CODE_V]	= GLFW_KEY_V,
	[KEY_CODE_W]	= GLFW_KEY_W,
	[KEY_CODE_X]	= GLFW_KEY_X,
	[KEY_CODE_Y]	= GLFW_KEY_Y,
	[KEY_CODE_Z]	= GLFW_KEY_Z,

	// Numbers
	[KEY_CODE_0] = GLFW_KEY_0,
	[KEY_CODE_1] = GLFW_KEY_1,
	[KEY_CODE_2] = GLFW_KEY_2,
	[KEY_CODE_3] = GLFW_KEY_3,
	[KEY_CODE_4] = GLFW_KEY_4,
	[KEY_CODE_5] = GLFW_KEY_5,
	[KEY_CODE_6] = GLFW_KEY_6,
	[KEY_CODE_7] = GLFW_KEY_7,
	[KEY_CODE_8] = GLFW_KEY_8,
	[KEY_CODE_9] = GLFW_KEY_9,

	// Function Keys
	[KEY_CODE_ESCAPE]		 = GLFW_KEY_ESCAPE,
	[KEY_CODE_LEFT_CONTROL]	 = GLFW_KEY_LEFT_CONTROL,
	[KEY_CODE_LEFT_SHIFT]	 = GLFW_KEY_LEFT_SHIFT,
	[KEY_CODE_LEFT_ALT]		 = GLFW_KEY_LEFT_ALT,
	[KEY_CODE_LEFT_SUPER]	 = GLFW_KEY_LEFT_SUPER,
	[KEY_CODE_RIGHT_CONTROL] = GLFW_KEY_RIGHT_CONTROL,
	[KEY_CODE_RIGHT_SHIFT]	 = GLFW_KEY_RIGHT_SHIFT,
	[KEY_CODE_RIGHT_ALT]	 = GLFW_KEY_RIGHT_ALT,
	[KEY_CODE_RIGHT_SUPER]	 = GLFW_KEY_RIGHT_SUPER,
	[KEY_CODE_MENU]			 = GLFW_KEY_MENU,
	[KEY_CODE_SPACE]		 = GLFW_KEY_SPACE,
	[KEY_CODE_ENTER]		 = GLFW_KEY_ENTER,
	[KEY_CODE_BACKSPACE]	 = GLFW_KEY_BACKSPACE,
	[KEY_CODE_TAB]			 = GLFW_KEY_TAB,
	[KEY_CODE_CAPS_LOCK]	 = GLFW_KEY_CAPS_LOCK,

	// Function keys
	[KEY_CODE_F1]			= GLFW_KEY_F1,
	[KEY_CODE_F2]			= GLFW_KEY_F2,
	[KEY_CODE_F3]			= GLFW_KEY_F3,
	[KEY_CODE_F4]			= GLFW_KEY_F4,
	[KEY_CODE_F5]			= GLFW_KEY_F5,
	[KEY_CODE_F6]			= GLFW_KEY_F6,
	[KEY_CODE_F7]			= GLFW_KEY_F7,
	[KEY_CODE_F8]			= GLFW_KEY_F8,
	[KEY_CODE_F9]			= GLFW_KEY_F9,
	[KEY_CODE_F10]			= GLFW_KEY_F10,
	[KEY_CODE_F11]			= GLFW_KEY_F11,
	[KEY_CODE_F12]			= GLFW_KEY_F12,
	[KEY_CODE_PRINT_SCREEN] = GLFW_KEY_PRINT_SCREEN,
	[KEY_CODE_SCROLL_LOCK]	= GLFW_KEY_SCROLL_LOCK,
	[KEY_CODE_PAUSE]		= GLFW_KEY_PAUSE,
	[KEY_CODE_INSERT]		= GLFW_KEY_INSERT,
	[KEY_CODE_DELETE]		= GLFW_KEY_DELETE,
	[KEY_CODE_HOME]			= GLFW_KEY_HOME,
	[KEY_CODE_END]			= GLFW_KEY_END,

	// Arrow keys
	[KEY_CODE_PAGE_UP]	   = GLFW_KEY_PAGE_UP,
	[KEY_CODE_PAGE_DOWN]   = GLFW_KEY_PAGE_DOWN,
	[KEY_CODE_ARROW_UP]	   = GLFW_KEY_UP,
	[KEY_CODE_ARROW_DOWN]  = GLFW_KEY_DOWN,
	[KEY_CODE_ARROW_LEFT]  = GLFW_KEY_LEFT,
	[KEY_CODE_ARROW_RIGHT] = GLFW_KEY_RIGHT,

	// Numpad keys
	[KEY_CODE_NUMPAD_0]		   = GLFW_KEY_KP_0,
	[KEY_CODE_NUMPAD_1]		   = GLFW_KEY_KP_1,
	[KEY_CODE_NUMPAD_2]		   = GLFW_KEY_KP_2,
	[KEY_CODE_NUMPAD_3]		   = GLFW_KEY_KP_3,
	[KEY_CODE_NUMPAD_4]		   = GLFW_KEY_KP_4,
	[KEY_CODE_NUMPAD_5]		   = GLFW_KEY_KP_5,
	[KEY_CODE_NUMPAD_6]		   = GLFW_KEY_KP_6,
	[KEY_CODE_NUMPAD_7]		   = GLFW_KEY_KP_7,
	[KEY_CODE_NUMPAD_8]		   = GLFW_KEY_KP_8,
	[KEY_CODE_NUMPAD_9]		   = GLFW_KEY_KP_9,
	[KEY_CODE_NUMPAD_ADD]	   = GLFW_KEY_KP_ADD,
	[KEY_CODE_NUMPAD_SUBTRACT] = GLFW_KEY_KP_SUBTRACT,
	[KEY_CODE_NUMPAD_MULTIPLY] = GLFW_KEY_KP_MULTIPLY,
	[KEY_CODE_NUMPAD_DIVIDE]   = GLFW_KEY_KP_DIVIDE,
	[KEY_CODE_NUMPAD_DECIMAL]  = GLFW_KEY_KP_DECIMAL,
	[KEY_CODE_NUMPAD_ENTER]	   = GLFW_KEY_KP_ENTER,

};

constexpr i32 GLFWMouseButtonMaps[MOUSE_BUTTON_CODE_COUNT] = {
	[MOUSE_BUTTON_CODE_NONE]   = GLFW_MOUSE_BUTTON_LAST + 1,
	[MOUSE_BUTTON_CODE_LEFT]   = GLFW_MOUSE_BUTTON_LEFT,
	[MOUSE_BUTTON_CODE_RIGHT]  = GLFW_MOUSE_BUTTON_RIGHT,
	[MOUSE_BUTTON_CODE_MIDDLE] = GLFW_MOUSE_BUTTON_MIDDLE,
};

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

	for (u32 keyCodeIndex = 0; keyCodeIndex < KEY_CODE_COUNT; ++keyCodeIndex)
	{
		Input::m_previousKeyStates[keyCodeIndex] = Input::m_keyStates[keyCodeIndex];
		Input::m_keyStates[keyCodeIndex] =
			glfwGetKey(m_pSurface->GetGLFWWindow(), static_cast<i32>(GLFWKeyMaps[keyCodeIndex])) == GLFW_PRESS
				? NTT_TRUE
				: NTT_FALSE;
	}

	for (u32 buttonCodeIndex = 0; buttonCodeIndex < MOUSE_BUTTON_CODE_COUNT; ++buttonCodeIndex)
	{
		Input::m_previousMouseButtonStates[buttonCodeIndex] = Input::m_mouseButtonStates[buttonCodeIndex];
		Input::m_mouseButtonStates[buttonCodeIndex] =
			glfwGetMouseButton(m_pSurface->GetGLFWWindow(), static_cast<i32>(GLFWMouseButtonMaps[buttonCodeIndex])) ==
					GLFW_PRESS
				? NTT_TRUE
				: NTT_FALSE;
	}
}

void Window::Close()
{
	NTT_ASSERT(m_pSurface != nullptr);
	glfwSetWindowShouldClose(m_pSurface->GetGLFWWindow(), GLFW_TRUE);
}

void InitializeWindowingSystem()
{
	NTT_ASSERT(glfwInit());

#if NTT_USE_GRAPHICS_OPENGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
}

void ShutdownWindowingSystem()
{
	glfwTerminate();
}

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL