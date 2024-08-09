#include "pch.h"
#include "Graphics.h"

Graphics* Graphics::graphic;

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		break;
		
		/*
		case WM_PAINT:
		{
			if (Graphics::GetInstance() != nullptr)
			{
				HWND hwnd = Graphics::GetInstance()->hWnd;

				float dpi = GetDpiForWindow(hwnd);

				SetWindowPos(
					hwnd,
					NULL,
					NULL,
					NULL,
					static_cast<int>(ceil(1600.f * dpi / 96.f)),
					static_cast<int>(ceil(900.f * dpi / 96.f)),
					SWP_NOMOVE);
				ShowWindow(hwnd, SW_SHOWNORMAL);
				UpdateWindow(hwnd);
			}
		}
		break;
		*/
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

Graphics::Graphics() :
	hWnd(0),
	pFactory(nullptr),
	pRenderTarget(nullptr),
	writeFactory(nullptr),
	textFormat(nullptr),
	brush(nullptr),
	Geometry(nullptr),
	Bitmap(nullptr),
	pIWICFactory(nullptr)
{
	WindowInitialize();
	CreateDeviceResources();
}

Graphics::~Graphics()
{
	SafeRelease(&pFactory);
	SafeRelease(&pRenderTarget);
	SafeRelease(&writeFactory);
	SafeRelease(&textFormat);
	SafeRelease(&brush);
	SafeRelease(&Geometry);
	SafeRelease(&Bitmap);
	SafeRelease(&pIWICFactory);
	if (graphic != nullptr)
	{
		delete graphic;
	}
}

void Graphics::WindowInitialize()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // 프로시저
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = 0;
	wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"KH_Engine";
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	hWnd = CreateWindow(
		wcex.lpszClassName,
		L"KH_Engine",
		WS_OVERLAPPEDWINDOW /*WS_OVERLAPPED*/ | WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT,		// 윈도우 좌측 상단의 x 좌표
		CW_USEDEFAULT,					// 윈도우 좌측 상단의 y 좌표
		0,		// 윈도우 가로 방향 해상도
		0,					// 윈도우 세로 방향 해상도
		NULL,
		NULL,
		wcex.hInstance,
		NULL);

	if (hWnd)
	{
		float dpi = GetDpiForWindow(hWnd);

		SetWindowPos(
			hWnd,
			NULL,
			NULL,
			NULL,
			static_cast<int>(ceil(1600.f * dpi / 96.f)),
			static_cast<int>(ceil(900.f * dpi / 96.f)),
			SWP_NOMOVE);
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}
}

void Graphics::CreateDeviceResources()
{
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

	if (SUCCEEDED(hr))
	{
		// std::cout << "팩토리 초기화 성공" << std::endl;

		RECT rc;
		GetClientRect(hWnd, &rc);
		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right, rc.bottom)),
			&pRenderTarget);
	}

	// IDWriteFactory 생성
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	
	if (SUCCEEDED(hr))
	{
		// IDWriteTextFormat 생성
		hr = writeFactory->CreateTextFormat(L"consolas", NULL,
			DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			13.f, L"en-us", &textFormat);
	}

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER, 
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&pIWICFactory));
}


void Graphics::BeginDraw(D2D1::ColorF bgColor)
{
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(D2D1::ColorF(bgColor));
}

void Graphics::EndDraw()
{
	pRenderTarget->EndDraw();
}

void Graphics::SetFont(float fontsize,
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
		fontsize, L"consolas", &textFormat);
}

void Graphics::SetBrush(D2D1::ColorF color)
{
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &brush);
}

void Graphics::DrawLine(const std::vector<D2D1_POINT_2F>& points, D2D1::ColorF color)
{
	SetBrush(color);
	pRenderTarget->DrawLine(
		D2D1::Point2F(points[0].x, points[0].y),
		D2D1::Point2F(points[1].x, points[1].y),
		brush);
	brush->Release();

	// pRenderTarget->DrawGlyphRun()
}

void Graphics::DrawDynamicLine(const std::vector<D2D1_POINT_2F>& points, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF color)
{
	SetBrush(color);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->DrawLine(
		D2D1::Point2F(points[0].x, points[0].y),
		D2D1::Point2F(points[1].x, points[1].y),
		brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawRect(const std::vector<D2D1_POINT_2F>& points, D2D1::ColorF brushColor)
{
	int index = points.size() - 1;
	D2D1_POINT_2F LT;
	D2D1_POINT_2F RB;
	if (index > 2)
	{
		LT.x = points[0].x;
		LT.y = points[0].y;
		RB.x = points[3].x;
		RB.y = points[3].y;
	}
	else
	{
		LT.x = points[0].x;
		LT.y = points[0].y;
		RB.x = points[1].x;
		RB.y = points[1].y;
	}
	SetBrush(brushColor);
	pRenderTarget->DrawRectangle(
		D2D1::RectF(LT.x, LT.y,
			RB.x, RB.y), brush);
	brush->Release();
}

void Graphics::DrawFillRect(const std::vector<D2D1_POINT_2F>& points, D2D1::ColorF brushColor)
{
	int index = points.size() - 1;
	D2D1_POINT_2F LT;
	D2D1_POINT_2F RB;
	if (index > 2)
	{
		LT.x = points[0].x;
		LT.y = points[0].y;
		RB.x = points[3].x;
		RB.y = points[3].y;
	}
	else
	{
		LT.x = points[0].x;
		LT.y = points[0].y;
		RB.x = points[1].x;
		RB.y = points[1].y;
	}
	SetBrush(brushColor);
	pRenderTarget->FillRectangle(
		D2D1::RectF(LT.x, LT.y,
			RB.x, RB.y), brush);
	brush->Release();
}

void Graphics::DrawDynamicRect(const std::vector<D2D1_POINT_2F>& points, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->DrawRectangle(
		D2D1::RectF(points[0].x, points[0].y,
			points[2].x, points[2].y), brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawDynamicFillRect(const std::vector<D2D1_POINT_2F>& points, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->FillRectangle(
		D2D1::RectF(points[0].x, points[0].y,
			points[2].x, points[2].y), brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawPolygon(const std::vector<D2D1_POINT_2F>& points, D2D1::ColorF color)
{
	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			// std::cout << "pSink 열기 성공" << std::endl;

			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(points[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&points[0], points.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRenderTarget->DrawGeometry(Geometry, brush);
	brush->Release();
}

void Graphics::DrawFillPolygon(const std::vector<D2D1_POINT_2F>& points, D2D1::ColorF color)
{
	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(points[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&points[0], points.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRenderTarget->FillGeometry(Geometry, brush);
	brush->Release();
}

void Graphics::DrawDynamicPolygon(const std::vector<D2D1_POINT_2F>& points, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF color)
{
	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(points[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&points[0], points.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->DrawGeometry(Geometry, brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawDynamicFillPolygon(const std::vector<D2D1_POINT_2F>& points, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF color)
{
	hr = pFactory->CreatePathGeometry(&Geometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = Geometry->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(points[0],
				D2D1_FIGURE_BEGIN_FILLED);

			pSink->AddLines(&points[0], points.size());
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();

		pSink->Release();
	}

	SetBrush(color);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->FillGeometry(Geometry, brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawCircle(const D2D1_POINT_2F& position, const float& radius, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->DrawEllipse(D2D1::Ellipse(position, radius, radius), brush);
	brush->Release();
}

void Graphics::DrawFillCircle(const D2D1_POINT_2F& position, const float& radius, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->FillEllipse(D2D1::Ellipse(position, radius, radius), brush);
	brush->Release();
}

void Graphics::DrawEllipse(const D2D1_POINT_2F& position, const D2D1_POINT_2F& radius, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->DrawEllipse(D2D1::Ellipse(position, radius.x, radius.y), brush);
	brush->Release();
}

void Graphics::DrawFillEllipse(const D2D1_POINT_2F& position, const D2D1_POINT_2F& radius, D2D1::ColorF brushColor)
{
	SetBrush(brushColor);
	pRenderTarget->FillEllipse(D2D1::Ellipse(position, radius.x, radius.x), brush);
	brush->Release();
}

void Graphics::DrawDynamicEllipse(const D2D1_POINT_2F& position, const D2D1_POINT_2F& radius, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF brushColor /*= D2D1::ColorF::Black*/)
{
	SetBrush(brushColor);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->DrawEllipse(D2D1::Ellipse(position, radius.x, radius.y), brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::DrawDynamicFillEllipse(const D2D1_POINT_2F& position, const D2D1_POINT_2F& radius, const D2D1_MATRIX_3X2_F& Transform, D2D1::ColorF brushColor /*= D2D1::ColorF::Black*/)
{
	SetBrush(brushColor);
	pRenderTarget->SetTransform(Transform);
	pRenderTarget->FillEllipse(D2D1::Ellipse(position, radius.x, radius.x), brush);
	pRenderTarget->SetTransform(D2D1::IdentityMatrix());
	brush->Release();
}

void Graphics::PrintString(
	std::wstring wstr, float x, float y, int size, 
	D2D1::ColorF brushColor,
	DWRITE_FONT_WEIGHT fointWeight,
	DWRITE_FONT_STYLE fontStyle,
	DWRITE_FONT_STRETCH fontStretch)
{
	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	SetFont(size);
	SetBrush(brushColor);

	pRenderTarget->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
	textFormat->Release();
	brush->Release();
}

void Graphics::PrintValue(
	int value, float x, float y, int size, 
	D2D1::ColorF brushColor, 
	DWRITE_FONT_WEIGHT fointWeight,
	DWRITE_FONT_STYLE fontStyle,
	DWRITE_FONT_STRETCH fontStretch)
{
	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	char buff[20] = { '0' };
	sprintf_s(buff, "%d", value);

	std::string str = buff;
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());

	SetFont(size);
	SetBrush(brushColor);

	pRenderTarget->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
	textFormat->Release();
	brush->Release();
}

void Graphics::PrintFValue(
	float value, float x, float y, int size,
	D2D1::ColorF brushColor,
	DWRITE_FONT_WEIGHT fointWeight,
	DWRITE_FONT_STYLE fontStyle,
	DWRITE_FONT_STRETCH fontStretch)
{
	D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

	char buff[20] = { '0' };
	sprintf_s(buff, "%f", value);

	std::string str = buff;
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());

	SetFont(size);
	SetBrush(brushColor);

	pRenderTarget->DrawText(
		wstr.c_str(),
		wstr.size(),
		textFormat,
		D2D1::RectF(x, y, rtSize.width, rtSize.height),
		brush
	);
	textFormat->Release();
	brush->Release();
}

HRESULT Graphics::LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	// Bitmap Decoder를 만든다.
	hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);

	// 이미지 프레임을 얻어와서 IWICBitmapFrameDecode 개체에 저장함.
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	// D2D에 맞는 형식으로 변환하기 위해 IWICFormatConverter개체를 만듭니다.
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	// 비트맵의 크기를 조절함.
	// MSDN에 
	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{

			// 원본 크기와 요청한 크기를 비교해서 비율을 얻어낸다.
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				// 이미지 크기 조정에 쓰일  IWICBitmapScaler를 만든다.
				hr = pIWICFactory->CreateBitmapScaler(&pScaler);

				// 크기 만큼 이미지를 조정한다.
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
				}
				// D2D에서 사용할 수 있는 형식으로 변환되어야 하기 위해 32bppPBGRA로 변환합니다.
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut   // 픽셀 보간 모드
					);
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}
	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

HRESULT Graphics::LoadResourceBitmap(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// 리소스 파일에서 이미지를 로드.
	imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
	hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	// 리소스를 잠그고 이미지 크기를 계산한다.
	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;

	}

	// IWICStream을 개체를 만듬.
	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = pIWICFactory->CreateStream(&pStream);
	}
	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	// IWICBitmapDecoder를 만든다
	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = pIWICFactory->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	// 이미지에서 프레임을 검색하고 IWICBitmapFrameDecode 개체에 저장함.
	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	/// 32bppPBGRA 형식으로 변환하기 위해 IWICFormatConverter 개체를 들고 변환합니다.
	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an
		// IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
					if (SUCCEEDED(hr))
					{
						hr = pConverter->Initialize(
							pScaler,
							GUID_WICPixelFormat32bppPBGRA,
							WICBitmapDitherTypeNone,
							NULL,
							0.f,
							WICBitmapPaletteTypeMedianCut
						);
					}
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}
	/// 랜더 타겟에서 다른 D2D 개체와 함게 사용할 수 있는 ID2DBitmap 개체를 만듭니다.
	if (SUCCEEDED(hr))
	{
		//create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

void Graphics::LoadBitmapFile(std::wstring wstr)
{
	ID2D1Bitmap* pBitmap;

	hr = LoadBitmapFromFile(
		pRenderTarget,
		pIWICFactory,
		wstr.c_str(),
		0,
		0,
		&pBitmap);

	if (SUCCEEDED(hr))
	{
		BitmapList.push_back(pBitmap);
	}
}

void Graphics::DrawBitmap(int index, int x, int y, int width, int height)
{
	int a = BitmapList.size() - 1;
	if (a < index)
	{
		return;
	}

	if (BitmapList[index] != NULL)
	{
		pRenderTarget->DrawBitmap(
			BitmapList[index],
			D2D1::RectF(x, y, x + width, y + height),		// 그릴 크기
			1.f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			//D2D1::RectF(x, y, x + 100, y + 100)			// 원본 위치
			NULL
			);
	}
	else
	{
		PrintString(L"이미지 오류", x, y, 20);
	}
}

/// 교수님의 가변인자 함수
/*
bool DRD2DEngine::DrawText(int x, int y, const char* pch, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리
	char* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// 당연히 멀티바이트와 유니코드의 문자열 길이 차이가 있다. 2바이트 문자 등
	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// 폰트별 차지하는 영역 계산 필요
		m_pRenderTarget->DrawText(unicodestr, wcslen(unicodestr), m_pTextFormat,
			//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
			D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
			m_pNowBrush);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
}
*/