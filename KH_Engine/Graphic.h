#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "dwrite")

#include <vector>
#include <string>

class Vector2D;

class Graphic
{
private:
	Graphic();
	Graphic(WinInIt(HINSTANCE, std::wstring));
	~Graphic();

private:
	HWND m_hWnd;
	HRESULT hr;

	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRT;

	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* brush;
	ID2D1PathGeometry* Geometry;

	void CreateDeviceResources();

public:
	static Graphic& GetInstance();

	void Init(HWND& refhWnd)
	{
		this->m_hWnd = refhWnd;
	}


public:
	void BeginDraw(D2D1::ColorF bgColor = D2D1::ColorF::White);

	void SetFont(float fontsize,											// 폰트 설정
		std::wstring fontName = L"Verdana",
		DWRITE_FONT_WEIGHT fointWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);

	void SetBrush(D2D1::ColorF color = D2D1::ColorF::Black);				// 브러시색 설정
	void EndDraw();



	// void DrawLine(const Line& line, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawLine(const std::vector<Vector2D>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);


	// void DrawRect(const Box& box, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawRect(															// 사각형 그리기
		const std::vector<Vector2D>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	// void DrawFillRect(const Box& box, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawFillRect(
		const std::vector<Vector2D>& points,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	// void DrawPolygon(const Box& box, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawPolygon(
		const std::vector<Vector2D>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);

	// void DrawFillPolygon(const Box& box, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawFillPolygon(
		const std::vector<Vector2D>& points,
		D2D1::ColorF color = D2D1::ColorF::Black);



	// void DrawCircle(const Circle& circle, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawCircle(
		const Vector2D& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	// void DrawFillCircle(const Circle& circle, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawFillCircle(
		const Vector2D& position,
		const float& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);



	// void DrawEllipse(const Circle& circle, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawEllipse(
		const Vector2D& position,
		const Vector2D& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	// void DrawFillEllipse(const Circle& circle, D2D1::ColorF color = D2D1::ColorF::Black);
	void DrawFillEllipse(
		const Vector2D& position,
		const Vector2D& radius,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);



	void PrintString(
		std::wstring wstr,
		int x, int y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void PrintValue(
		int value,
		int x, int y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);

	void PrintFValue(
		float value,
		int x, int y,
		int size,
		D2D1::ColorF brushColor = D2D1::ColorF::Black);
	

};
