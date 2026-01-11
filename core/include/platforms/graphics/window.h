#pragma once
#include "platforms/common.h"
#include "surface.h"

namespace ntt {

class Window
{
public:
	Window(const char* title, u32 width, u32 height);
	NTT_DELETE_COPY(Window)
	NTT_DELETE_MOVE(Window)
	~Window();

public:
	/**
	 * Only be called if the window is being created.
	 */
	void Initialize();

	/**
	 * Destroy the window and its associated resources.
	 */
	void Shutdown();

	/**
	 * Called at each frame to poll for window events (like input, close, resize, etc.).
	 */
	void PollEvents();

	/**
	 * Be used for checking if the window is still open or has been closed.
	 */
	inline b8 IsOpen() const
	{
		return m_isOpen;
	}

	inline Reference<Surface>& GetSurface()
	{
		return m_pSurface;
	}

private:
	u32	   m_width;
	u32	   m_height;
	String m_title;

private:
	b8				   m_isOpen;
	Reference<Surface> m_pSurface;

private:
	class WindowInternal;
	Scope<WindowInternal> m_internal;
};

void InitializeWindowingSystem();
void ShutdownWindowingSystem();

} // namespace ntt
