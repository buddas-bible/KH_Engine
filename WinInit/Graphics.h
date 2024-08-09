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

	HWND					hWnd;				// �׸� ������
private:
	HRESULT					hr;
	ID2D1Factory*			pFactory;			// ���丮
	ID2D1HwndRenderTarget*	pRenderTarget;		// ���� Ÿ��
	IDWriteFactory*			writeFactory;		// �������� ���丮
	IDWriteTextFormat*		textFormat;			// �ؽ�Ʈ ����
	ID2D1SolidColorBrush*	brush;				// �귯��
	ID2D1PathGeometry*		Geometry;			// �ٰ����� �׸��� ���� ��
	ID2D1Bitmap*			Bitmap;				// ��Ʈ��

	/// ������ �ʱ�ȭ
	void WindowInitialize();
	
	/// D2D ���ҽ� �ʱ�ȭ
	void CreateDeviceResources();

	/// 
	static Graphics* graphic;

public:
	/// ��ü ���� ������ ���� �Լ�
	static Graphics* CreateInstance()
	{
		if (graphic == nullptr)
		{
			graphic = new Graphics();
		}
		return graphic;
	}
	
	/// �̱��� ��ü�� �ޱ����� �Լ�
	static Graphics* GetInstance()
	{
		return graphic;
	}


public:
	/// �׸��� ���� �Լ�
#pragma region �׸��� �Լ�
	void BeginDraw(D2D1::ColorF bgColor = D2D1::ColorF::White);
	void EndDraw();

	/// ��Ʈ ����
	void SetFont(float fontsize,
		std::wstring fontName = L"Cascadia Mono",
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// �귯�� �� ����
	void SetBrush(D2D1::ColorF color = D2D1::ColorF::Black);



	/// �� �׸���
	void DrawLine(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	void DrawDynamicLine(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF color = D2D1::ColorF::Black);

	/// ���� �׸���
	void DrawRect(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void DrawFillRect(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// ȸ���ϴ� �簢�� 
	void DrawDynamicRect(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// ��ĥ�� ���� �׸���
	void DrawDynamicFillRect(
		const std::vector<D2D1_POINT_2F>& points,
		const D2D1_MATRIX_3X2_F& Transform,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);


	/// �ٰ��� �׸���
	void DrawPolygon(
		const std::vector<D2D1_POINT_2F>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	/// ��ĥ�� �ٰ��� �׸���
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


	/// �� �׸���
	void DrawCircle(
		const D2D1_POINT_2F& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// ��ĥ�� �� �׸���
	void DrawFillCircle(
		const D2D1_POINT_2F& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);


	/// Ÿ�� �׸���
	void DrawEllipse(
		const D2D1_POINT_2F& position,
		const D2D1_POINT_2F& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	/// ��ĥ�� Ÿ�� �׸���
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

	/// ���ڿ� ����ϱ�
	void PrintString(
		std::wstring wstr,
		float x, float y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// ���� ����ϱ�
	void PrintValue(
		int value,
		float x, float y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	/// �Ǽ� ����ϱ�
	void PrintFValue(
		float value,
		float x, float y, int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black,
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);
#pragma endregion �׸��� �Լ�

/// ��Ʈ�� ���� �͵�
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