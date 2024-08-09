#pragma once


class InputManager
{
public:
	void Update();

	// 키의 눌렸는지를 리턴한다.
	bool PressedKey(int keycode);
	// 키가 지금 눌렸는지를 리턴한다.
	bool PressedKeyNow(int keycode);
	// 키가 떨어졌는지 리턴한다.
	bool ReleasedKey(int keycode);
	// 키가 지금 떨어졌는지 리턴한다.
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

