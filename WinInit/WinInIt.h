#pragma once

#include "windows.h"

#include <string>
#include <vector>

class WinInIt
{
public:
	WinInIt();
	WinInIt(HINSTANCE& hInstance, std::wstring wstr);
	~WinInIt();

private:
	HWND hWnd;
	HRESULT hr;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	ATOM WindowInitialize(HINSTANCE& hInstance, std::wstring wstr, int bgColor = 0);

public:
	HWND& GetHWND();
};
