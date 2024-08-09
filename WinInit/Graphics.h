#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

#include "Sprite.h"

#include <vector>
#include <string>

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceRelease)
{
	if (*ppInterfaceRelease != nullptr)
	{
		(*ppInterfaceRelease)->Release();
		(*ppInterfaceRelease) = nullptr;
	}
}

class Graphics
{
private:
	Graphics();

public:
	~Graphics();

	HWND					hWnd;				// 그릴 윈도우
private:
	HRESULT					hr;
	ID2D1Factory*			pFactory;			// 팩토리
	ID2D1HwndRenderTarget*	pRenderTarget;		// 랜더 타겟
	IDWriteFactory*			writeFactory;		// 쓰기위한 팩토리
	IDWriteTextFormat*		textFormat;			// 텍스트 설정
	ID2D1SolidColorBrush*	brush;				// 브러쉬
	ID2D1PathGeometry*		Geometry;			// 다각형을 그리기 위한 것
	ID2D1Bitmap*			Bitmap;				// 비트맵

	/// 윈도우 초기화
	void WindowInitialize();
	
	/// D2D 리소스 초기화
	void CreateDeviceResources();

	/// 
	static Graphics* graphic;

public:
	/// 객체 최초 생성을 위한 함수
	static Graphics* CreateInstance()
	{
		if (graphic == nullptr)
		{
			graphic = new Graphics();
		}
		return graphic;
	}
	
	/// 싱글턴 객체를 받기위한 함수
	static Graphics* GetInstance()
	{
		return graphic;
	}


public:
	/// 그리기 시작 함수
#pragma region 그리기 함수
	void BeginDraw(D2D1::ColorF bgColor = D2D1::ColorF::White);
	void EndDraw();

	/// 폰트 설정
	void SetFont(float fontsize,
		std::wstring fontName = L"Cascadia Mono",
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// 브러시 색 설정
	void SetBrush(D2D1::ColorF color = D2D1::ColorF::Black);



	/// 선 그리기
	void DrawLine(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	void DrawDynamicLine(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF color = D2D1::ColorF::Black);

	/// 상자 그리기
	void DrawRect(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void DrawFillRect(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// 회전하는 사각형 
	void DrawDynamicRect(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// 색칠된 상자 그리기
	void DrawDynamicFillRect(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);


	/// 다각형 그리기
	void DrawPolygon(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	/// 색칠된 다각형 그리기
	void DrawFillPolygon(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	void DrawDynamicPolygon(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF color = D2D1::ColorF::Black);

	void DrawDynamicFillPolygon(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF color = D2D1::ColorF::Black);


	/// 원 그리기
	void DrawCircle(
		const D2D1_POINT_2F& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// 색칠된 원 그리기
	void DrawFillCircle(
		const D2D1_POINT_2F& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);


	/// 타원 그리기
	void DrawEllipse(
		const D2D1_POINT_2F& position,
		const D2D1_POINT_2F& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// 색칠된 타원 그리기
	void DrawFillEllipse(
		const D2D1_POINT_2F& position,
		const D2D1_POINT_2F& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void DrawDynamicEllipse(
		const D2D1_POINT_2F& position,
		const D2D1_POINT_2F& radius,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void DrawDynamicFillEllipse(
		const D2D1_POINT_2F& position,
		const D2D1_POINT_2F& radius,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// 문자열 출력하기
	void PrintString(
		std::wstring wstr,
		float x, float y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// 정수 출력하기
	void PrintValue(
		int value,
		float x, float y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// 실수 출력하기
	void PrintFValue(
		float value,
		float x, float y, int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);
#pragma endregion 그리기 함수

/// 비트맵 관련 것들
private:
		IWICImagingFactory* pIWICFactory;

		std::vector<ID2D1Bitmap*> BitmapList;

public:
	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap);

	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap);

public:
	void LoadBitmap(std::string str);
	void LoadBitmapFile(std::wstring wstr);
	void LoadSpriteFromSheet(char*);
	void LoadSpriteFromSheet(int);

	void DrawBitmap(int index, int x, int y, int width, int height);

	void DrawSprite(int);
	void DrawSprite(/**/);
};


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif