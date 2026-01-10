#pragma once
#include "keys.h"
#include "platforms/common.h"

namespace ntt {
class Window;

class Input
{
public:
	static void Initialize();
	static void Shutdown();

	static b8 IsKeyUp(KeyCode key);
	static b8 IsKeyDown(KeyCode key);
	static b8 IsKeyPressed(KeyCode key);
	static b8 IsKeyReleased(KeyCode key);

	static b8 IsMouseButtonUp(MouseButtonCode button);
	static b8 IsMouseButtonDown(MouseButtonCode button);
	static b8 IsMouseButtonPressed(MouseButtonCode button);
	static b8 IsMouseButtonReleased(MouseButtonCode button);

private:
	static b8 m_isInitialized;

	static b8 m_previousKeyStates[KEY_CODE_COUNT]; /// 0 for up, 1 for down
	static b8 m_keyStates[KEY_CODE_COUNT];		   /// 0 for up, 1 for down

	static b8 m_previousMouseButtonStates[MOUSE_BUTTON_CODE_COUNT]; /// 0 for up, 1 for down
	static b8 m_mouseButtonStates[MOUSE_BUTTON_CODE_COUNT];			/// 0 for up, 1 for down

public:
	friend class Window;
};

} // namespace ntt
