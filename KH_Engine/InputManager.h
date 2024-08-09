#pragma once


class InputManager
{
public:
	void Update();

	// Ű�� ���ȴ����� �����Ѵ�.
	bool PressedKey(int keycode);
	// Ű�� ���� ���ȴ����� �����Ѵ�.
	bool PressedKeyNow(int keycode);
	// Ű�� ���������� �����Ѵ�.
	bool ReleasedKey(int keycode);
	// Ű�� ���� ���������� �����Ѵ�.
	bool ReleasedKeyNow(int keycode);

private:
	bool currentKeyState[0xFF];
	bool lastKeyState[0xFF];

	//singleton
private:
	InputManager();
	~InputManager();
	InputManager(const InputManager& ref);
	InputManager& operator=(const InputManager& ref) {}

public:
	static InputManager& GetInstance();
};

