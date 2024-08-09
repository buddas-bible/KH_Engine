// WinInit.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "WinInIt.h"

WinInIt::WinInIt() : 
	hWnd(nullptr),
	hr(0)
{
}

WinInIt::WinInIt(HINSTANCE& hInstance, std::wstring wstr)
{
	WindowInitialize(hInstance, wstr);
}

WinInIt::~WinInIt()
{

}

LRESULT WinInIt::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}
}

ATOM WinInIt::WindowInitialize(HINSTANCE& hInstance, std::wstring wstr, int bgColor)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinInIt::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)bgColor;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = wstr.c_str();
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);


	float dpiX{ (FLOAT)GetDpiForWindow(GetDesktopWindow()) };
	float dpiY{ dpiX };

	hWnd = CreateWindowW(
		wstr.c_str(),
		wstr.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(1920.f * dpiX / 96.f)),
		static_cast<UINT>(ceil(1080.f * dpiY / 96.f)),
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	return TRUE;
}

HWND& WinInIt::GetHWND()
{
	return hWnd;
}
