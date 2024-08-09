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

/// 키가 눌렸는지 확인
bool InputManager::PressedKey(int keycode)
{
	return currentKeyState[keycode];
}

/// 키가 지금 눌렸는지 확인
bool InputManager::PressedKeyNow(int keycode)
{
	return currentKeyState[keycode] && !lastKeyState[keycode];
}

/// 키가 떨어졌는지 확인
bool InputManager::ReleasedKey(int keycode)
{
	return !currentKeyState[keycode];
}

/// 키가 지금 떨어졌는지 확인
bool InputManager::ReleasedKeyNow(int keycode)
{
	return !currentKeyState[keycode] && lastKeyState[keycode];
}
