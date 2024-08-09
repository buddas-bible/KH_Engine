#include "InputManager.h"
#include <windows.h>

InputManager::InputManager()
{
	Update();
}

InputManager::~InputManager()
{

}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::Update()
{
	for (size_t keycode = 0x01; keycode < 0xFF; keycode++)
	{
		lastKeyState[keycode] = currentKeyState[keycode];
		currentKeyState[keycode] = GetAsyncKeyState(keycode) & 0x8000;
	}
}

/// Ű�� ���ȴ��� Ȯ��
bool InputManager::PressedKey(int keycode)
{
	return currentKeyState[keycode];
}

/// Ű�� ���� ���ȴ��� Ȯ��
bool InputManager::PressedKeyNow(int keycode)
{
	return currentKeyState[keycode] && !lastKeyState[keycode];
}

/// Ű�� ���������� Ȯ��
bool InputManager::ReleasedKey(int keycode)
{
	return !currentKeyState[keycode];
}

/// Ű�� ���� ���������� Ȯ��
bool InputManager::ReleasedKeyNow(int keycode)
{
	return !currentKeyState[keycode] && lastKeyState[keycode];
}
