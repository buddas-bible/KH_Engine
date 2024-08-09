#include "Loop.h"
#pragma comment(lib, "Graphics.lib")


int main(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Graphics* graphic = Graphics::CreateInstance();
	Loop gameLoop;

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		gameLoop.MainLoop();
	}

	CoUninitialize();
	return 0;
}
