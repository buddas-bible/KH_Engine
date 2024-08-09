#include "Graphic.h"
#include "Vector2D.h"

Graphic::Graphic() :
	m_hWnd(0),
	pFactory(nullptr),
	pRT(nullptr),
	writeFactory(nullptr),
	textFormat(nullptr),
	brush(nullptr),
	Geometry(nullptr)
{
	CreateDeviceResources();
}

Graphic::~Graphic()
{
	if (pFactory != nullptr)
	{
		pFactory->Release();
	}
	if (pRT != nullptr)
	{
		pRT->Release();
	}
	if (writeFactory != nullptr)
	{
		writeFactory->Release();
	}
	if (textFormat != nullptr)
	{
		textFormat->Release();
	}
	if (brush != nullptr)
	{
		brush->Release();
	}
	if (Geometry != nullptr)
	{
		Geometry->Release();
	}
}

void Graphic::CreateDeviceResources()
{
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

	if (SUCCEEDED(hr))
	{
		// std::cout << "팩토리 초기화 성공" << std::endl;

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right, rc.bottom)),
			&pRT);

		if (SUCCEEDED(hr))
		{
			// std::cout << "렌더타켓 초기화 성공" << std::endl;
		}
	}

	// IDWriteFactory 생성
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if (SUCCEEDED(hr))
	{
		// std::cout << "쓰기 팩토리 초기화 성공" << std::endl;

		if (SUCCEEDED(hr))
		{
			// IDWriteTextFormat 생성
			hr = writeFactory->CreateTextFormat(L"Verdana", NULL,
				DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				13.f, L"en-us", &textFormat);
			if (SUCCEEDED(hr))
			{
				// std::cout << "쓰기 팩토리 포맷 초기화 성공" << std::endl;
			}
		}
	}
}

Graphic& Graphic::GetInstance()
{
	static Graphic Instance;
	return Instance;
}

void Graphic::BeginDraw(D2D1::ColorF bgColor)
{
	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(bgColor));
}

void Graphic::EndDraw()
{
	pRT->EndDraw();
}

void Graphic::SetFont(float fontsize,
	std::wstring fontName,
	DWRITE_FONT_WEIGHT fointWeight,
	DWRITE_FONT_STYLE fontStyle,
	DWRITE_FONT_STRETCH fontStretch)
{
	writeFactory->CreateTextFormat(
		fontName.c_str(),
		NULL,
		fointWeight,
		fontStyle,
		fontStretch,
		fontsize, L"en-us", &textFormat);
}

void Graphic::SetBrush(D2D1::ColorF color)
{
	pRT->CreateSolidColorBrush(D2D1::ColorF(color), &brush);
}

void Graphic::DrawLine(const std::vector<Vector2D>& points, D2D1::ColorF color)
{
	SetBrush(color);
	pRT->DrawLine(
		D2D1::Point2F(points[0].X, points[0].Y),
		D2D1::Point2F(points[1].X, points[1].Y),
		brush);
}

void Graphic::DrawRect(const std::vector<Vector2D>& points, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRT->DrawRectangle(
		D2D1::RectF(points[0].X, points[0].Y,
			points[3].X, points[3].Y), brush);
}

void Graphic::DrawFillRect(const std::vector<Vector2D>& points, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRT->FillRectangle(
		D2D1::RectF(points[0].X, points[0].Y,
			points[3].X, points[3].Y), brush);
}

void Graphic::DrawPolygon(const std::vector<Vector2D>& points, D2D1::ColorF color)
{
	std::vector<D2D1_POINT_2F> vpoint;
	for (int i = 0; i < points.size(); i++)
	{
		vpoint.push_back((D2D1::Point2F(points[i].X, points[i].Y)));
	}

	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			// std::cout << "pSink 열기 성공" << std::endl;

			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(vpoint[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&vpoint[0], vpoint.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRT->DrawGeometry(Geometry, brush);
}

void Graphic::DrawFillPolygon(const std::vector<Vector2D>& points, D2D1::ColorF color)
{
	std::vector<D2D1_POINT_2F> vpoint;
	for (int i = 0; i < points.size(); i++)
	{
		vpoint.push_back(D2D1::Point2F(points[i].X, points[i].Y));
	}

	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(vpoint[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&vpoint[0], vpoint.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRT->FillGeometry(Geometry, brush);
}

void Graphic::DrawCircle(const Vector2D& position, const float& radius, D2D1::ColorF brushColor)
{
	D2D1_POINT_2F center = { position.X, position.Y };
	SetBrush(brushColor);
	pRT->DrawEllipse(D2D1::Ellipse(center, radius, radius), brush);
}

void Graphic::DrawFillCircle(const Vector2D& position, const float& radius, D2D1::ColorF brushColor)
{
	D2D1_POINT_2F center = { position.X, position.Y };
	SetBrush(brushColor);
	pRT->FillEllipse(D2D1::Ellipse(center, radius, radius), brush);
}

void Graphic::DrawEllipse(const Vector2D& position, const Vector2D& radius, D2D1::ColorF brushColor)
{
	D2D1_POINT_2F center = { position.X, position.Y };
	SetBrush(brushColor);
	pRT->DrawEllipse(D2D1::Ellipse(center, radius.X, radius.Y), brush);
}

void Graphic::DrawFillEllipse(const Vector2D& position, const Vector2D& radius, D2D1::ColorF brushColor)
{
	D2D1_POINT_2F center = { position.X, position.Y };
	SetBrush(brushColor);
	pRT->FillEllipse(D2D1::Ellipse(center, radius.X, radius.Y), brush);
}

void Graphic::PrintString(std::wstring wstr, int x, int y, int size, D2D1::ColorF brushColor)
{
	D2D1_SIZE_F rtSize = pRT->GetSize();

	SetFont(size);
	SetBrush(brushColor);

	pRT->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
}

void Graphic::PrintValue(int value, int x, int y, int size, D2D1::ColorF brushColor)
{
	D2D1_SIZE_F rtSize = pRT->GetSize();

	char buff[20] = { '0' };
	sprintf_s(buff, "%d", value);

	std::string str = buff;
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());

	SetFont(size);
	SetBrush(brushColor);

	pRT->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
}

void Graphic::PrintFValue(float value, int x, int y, int size, D2D1::ColorF brushColor)
{
	D2D1_SIZE_F rtSize = pRT->GetSize();

	char buff[20] = { '0' };
	sprintf_s(buff, "%f", value);

	std::string str = buff;
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());

	SetFont(size);
	SetBrush(brushColor);

	pRT->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
}