#if NTT_USE_GRAPHICS_OPENGL && NTT_USE_GLFW

#pragma once
#include "platforms/common.h"
#include <GLFW/glfw3.h>

namespace ntt {

class Surface
{
public:
	Surface(GLFWwindow* pWindow);
	NTT_DELETE_COPY(Surface)
	NTT_DELETE_MOVE(Surface)
	~Surface();

	inline GLFWwindow* GetGLFWWindow() const
	{
		return m_pWindow;
	}

	void Bind();

private:
	GLFWwindow* m_pWindow;
};

} // namespace ntt

#endif // NTT_USE_GRAPHICS_OPENGL && NTT_USE_GLFW
