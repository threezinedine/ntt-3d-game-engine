#include "input/manager.h"

namespace ntt {
b8 Input::m_isInitialized									   = NTT_FALSE;
b8 Input::m_previousKeyStates[KEY_CODE_COUNT]				   = {NTT_FALSE};
b8 Input::m_keyStates[KEY_CODE_COUNT]						   = {NTT_FALSE};
b8 Input::m_previousMouseButtonStates[MOUSE_BUTTON_CODE_COUNT] = {NTT_FALSE};
b8 Input::m_mouseButtonStates[MOUSE_BUTTON_CODE_COUNT]		   = {NTT_FALSE};

void Input::Initialize()
{
	NTT_ASSERT(!m_isInitialized);

	m_isInitialized = NTT_TRUE;
}

void Input::Shutdown()
{
	NTT_ASSERT(m_isInitialized);

	m_isInitialized = NTT_FALSE;
}

b8 Input::IsKeyDown(KeyCode key)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(key > KEY_CODE_NONE && key < KEY_CODE_COUNT);

	return m_keyStates[key];
}

b8 Input::IsKeyUp(KeyCode key)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(key > KEY_CODE_NONE && key < KEY_CODE_COUNT);

	return !m_keyStates[key];
}

b8 Input::IsKeyPressed(KeyCode key)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(key > KEY_CODE_NONE && key < KEY_CODE_COUNT);

	return m_keyStates[key] && !m_previousKeyStates[key];
}

b8 Input::IsKeyReleased(KeyCode key)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(key > KEY_CODE_NONE && key < KEY_CODE_COUNT);

	return !m_keyStates[key] && m_previousKeyStates[key];
}

b8 Input::IsMouseButtonDown(MouseButtonCode button)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(button > MOUSE_BUTTON_CODE_NONE && button < MOUSE_BUTTON_CODE_COUNT);

	return m_mouseButtonStates[button];
}

b8 Input::IsMouseButtonUp(MouseButtonCode button)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(button > MOUSE_BUTTON_CODE_NONE && button < MOUSE_BUTTON_CODE_COUNT);

	return !m_mouseButtonStates[button];
}

b8 Input::IsMouseButtonPressed(MouseButtonCode button)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(button > MOUSE_BUTTON_CODE_NONE && button < MOUSE_BUTTON_CODE_COUNT);

	return m_mouseButtonStates[button] && !m_previousMouseButtonStates[button];
}

b8 Input::IsMouseButtonReleased(MouseButtonCode button)
{
	NTT_ASSERT(m_isInitialized);
	NTT_ASSERT(button > MOUSE_BUTTON_CODE_NONE && button < MOUSE_BUTTON_CODE_COUNT);

	return !m_mouseButtonStates[button] && m_previousMouseButtonStates[button];
}

} // namespace ntt
