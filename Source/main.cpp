#include "Image/TgaImage.h"

#include "Model/Model.h"
#include "Renderer/DepthBuffer.h"
#include "Renderer/Rasterizer.h"
#include "Shaders/Shader_SimpleLitDiffuse.h"

#include <windows.h>

using namespace TV;
using namespace Maths;
using namespace Renderer;

constexpr Colour white = Colour(255, 255, 255, 255);
constexpr Colour red = Colour(255, 0, 0, 255);
constexpr Colour green = Colour(0, 255, 0, 255);

class WindowsCanvas : public ICanvas
{
public:
	WindowsCanvas(const Vec2i& size) : Size(size)
	{
		BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;
		BitmapInfo.bmiHeader.biWidth = size.X;
		BitmapInfo.bmiHeader.biHeight = size.Y;
		DeviceContext = CreateCompatibleDC(nullptr);
		Bitmap = CreateDIBSection(nullptr, &BitmapInfo, DIB_RGB_COLORS, (void**)&Pixels, nullptr, 0);
		if (Bitmap != nullptr)
		{
			OldBitmap = SelectObject(DeviceContext, Bitmap);
		}
		Clear(Colour(0, 0, 0));
	}
	~WindowsCanvas()
	{
		SelectObject(DeviceContext, OldBitmap);
		DeleteObject(Bitmap);
		DeleteObject(DeviceContext);
	}

	WindowsCanvas(const WindowsCanvas&) = delete;
	WindowsCanvas& operator = (const WindowsCanvas&) = delete;

	bool IsValid() const { return Pixels != nullptr && Bitmap != nullptr && DeviceContext != nullptr; }

	virtual Vec2i GetSize() const override { return Size; }
	virtual void SetPixel(const Vec2i& coord, const Colour& colour) override
	{
		ValidatePoint(coord);
		Pixels[coord.X + coord.Y * Size.X] = colour.GetARGB();
	}
	virtual Colour GetPixel(const Vec2i& coord) const override
	{
		ValidatePoint(coord);
		return Colour::MakeFromARGB(Pixels[coord.X + coord.Y * Size.X]);
	}

	void ValidatePoint(const Vec2i& point) const
	{
		check(Pixels != nullptr);
		check(point.X >= 0);
		check(point.Y >= 0);
		check(point.X < Size.X);
		check(point.Y < Size.Y);
	}

	void Clear(const Colour& clearColour)
	{
		for (int32 x = 0; x != Size.X; ++x)
		{
			for (int32 y = 0; y != Size.Y; ++y)
			{
				SetPixel(Vec2i(x, y), clearColour);
			}
		}
	}

	HDC GetDeviceContext() const { return DeviceContext; }

private:
	const Vec2i Size;
	uint32* Pixels = nullptr;
	HDC DeviceContext = nullptr;
	HGDIOBJ Bitmap = nullptr;
	HGDIOBJ OldBitmap = nullptr;
	BITMAPINFO BitmapInfo;
};

struct Globals
{
	Model _Model;
	TGAImage _ModelDiffuse;

	bool bLoaded = false;
	bool bQuit = false;
};
Globals g_globals;

struct RenderTargets
{
	DepthBuffer _DepthBuffer;
	WindowsCanvas _FrameBuffer;

	RenderTargets(const Vec2i& size) : _DepthBuffer(size), _FrameBuffer(size) {}

	void Clear()
	{
		_DepthBuffer.ClearBuffer();
		_FrameBuffer.Clear(Colour(0, 0, 0));
	}

	RenderContext GetRenderContext()
	{
		RenderContext context;
		context.Canvas = &_FrameBuffer;
		context.DepthBuffer = &_DepthBuffer;
		return context;
	}
};
RenderTargets* g_renderTargets = nullptr;

bool LoadResources()
{
	if (!g_globals._Model.LoadWavefrontFile("Content/african_head.obj"))
	{
		return false;
	}

	if (!g_globals._ModelDiffuse.read_tga_file("Content/african_head_diffuse.tga"))
	{
		return false;
	}
	g_globals._ModelDiffuse.flip_vertically();

	g_globals.bLoaded = true;
	return true;
}

void RenderModel(const RenderContext& renderContext, bool bWireframe)
{
	if (!renderContext.IsValid())
	{
		return;
	}

	const float aspectRatio = renderContext.Canvas->GetAspectRatio();

	TV::Shaders::Rasterizer_SimpleLitDiffuse rasterizer;

	// build camera matrix
	const Vec3f cameraPos(1.f, 1.f, 3.f);
	const Matrix4x4f cameraMtx = Matrix4x4f::MakeLookAt(cameraPos, Vec3f(), Vec3f::UpVector);
	rasterizer.ViewMatrix = cameraMtx.GetInverse();
	rasterizer.Diffuse = &g_globals._ModelDiffuse;

	// build projection matrix
	constexpr float nearclip = 0.1f;
	constexpr float farclip = 1000.f;
	constexpr bool bOrthographic = false;
	if (bOrthographic)
	{
		constexpr float orthoWidth = 2.f;
		rasterizer.ProjectionMatrix = Matrix4x4f::MakeOrthographicProjection(orthoWidth, aspectRatio, nearclip, farclip);
	}
	else
	{
		constexpr float verticalFieldOfView = GetRadiansFromDegrees(30.f);
		rasterizer.ProjectionMatrix = Matrix4x4f::MakePerspectiveProjection(verticalFieldOfView, aspectRatio, nearclip, farclip);
	}

	rasterizer.BaseColour = white;

	const Vec3f lightDir = Vec3f(1.f, 1.f, 1.f).GetSafeNormal();
	rasterizer.LightDirection = rasterizer.ViewMatrix.TransformVector(lightDir);

	if (bWireframe)
	{
		rasterizer.DrawModelWireframe(g_globals._Model, renderContext, white);
	}
	else
	{
		rasterizer.DrawModel(g_globals._Model, renderContext);
	}
}

void Render(HWND hwnd)
{
	if (!g_globals.bLoaded || g_renderTargets == nullptr)
	{
		return;
	}

	g_renderTargets->Clear();
	RenderModel(g_renderTargets->GetRenderContext(), false);

	static bool bDumpedBuffer = false;
	if (!bDumpedBuffer)
	{
		bDumpedBuffer = true;

		{
			TGAImage image(g_renderTargets->_FrameBuffer.GetSize().X, g_renderTargets->_FrameBuffer.GetSize().Y, TGAImage::RGB);
			for (int32 x = 0; x != image.get_width(); ++x)
			{
				for (int32 y = 0; y != image.get_height(); ++y)
				{
					image.SetPixel(Vec2i(x, y), g_renderTargets->_FrameBuffer.GetPixel(Vec2i(x, y)));
				}
			}
			image.flip_vertically();
			image.write_tga_file("framebuffer.tga");
		}
		{
			TGAImage image(g_renderTargets->_DepthBuffer.GetSize().X, g_renderTargets->_DepthBuffer.GetSize().Y, TGAImage::RGB);
			for (int32 x = 0; x != image.get_width(); ++x)
			{
				for (int32 y = 0; y != image.get_height(); ++y)
				{
					image.SetPixel(Vec2i(x, y), Colour((uint8)(255 * g_renderTargets->_DepthBuffer.Get(Vec2i(x, y))), 0, 0));
				}
			}
			image.flip_vertically();
			image.write_tga_file("depthbuffer.tga");
		}
	}

	PAINTSTRUCT paint;
	HDC deviceContext = BeginPaint(hwnd, &paint);
	BitBlt(deviceContext,
		paint.rcPaint.left, paint.rcPaint.top,
		paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
		g_renderTargets->_FrameBuffer.GetDeviceContext(),
		paint.rcPaint.left, paint.rcPaint.top,
		SRCCOPY);
	EndPaint(hwnd, &paint);
}

LRESULT CALLBACK WindowProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		g_globals.bQuit = true;
		break;

	case WM_PAINT:
		Render(hwnd);
		break;

	case WM_SIZE:
	{
		Vec2i windowSize;
		windowSize.X = LOWORD(lParam);
		windowSize.Y = HIWORD(lParam);

		if (g_renderTargets == nullptr || g_renderTargets->_FrameBuffer.GetSize() != windowSize)
		{
			if (windowSize.X > 0 && windowSize.Y > 0)
			{
				delete g_renderTargets;
				g_renderTargets = new RenderTargets(windowSize);
			}
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// http://www.winprog.org/tutorial/simple_window.html
	// https://croakingkero.com/tutorials/drawing_pixels_win32_gdi/

	LPCWSTR myWindowClassName = L"tinyRendererWindowClass";
	WNDCLASS windowClass = { 0 };
	windowClass.lpfnWndProc = WindowProcessMessage;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = myWindowClassName;
	if (!RegisterClass(&windowClass))
	{
		MessageBox(nullptr, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	constexpr Vec2i defaultWindowSize(600, 800);
	const HWND hwnd = CreateWindow(
		myWindowClassName,
		L"tinyrenderer",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, defaultWindowSize.X, defaultWindowSize.Y,
		nullptr, nullptr, hInstance, nullptr);
	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	if (!LoadResources())
	{
		MessageBox(nullptr, L"LoadResources failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// render image to files
	{
		TGAImage image(defaultWindowSize.X, defaultWindowSize.Y, TGAImage::RGB);
		DepthBuffer depthBuffer(defaultWindowSize);
		RenderContext renderContext;
		renderContext.Canvas = &image;
		renderContext.DepthBuffer = &depthBuffer;
		RenderModel(renderContext, false);
		image.flip_vertically();
		image.write_tga_file("output.tga");
	}
	{
		TGAImage image(defaultWindowSize.X, defaultWindowSize.Y, TGAImage::RGB);
		RenderContext renderContext;
		renderContext.Canvas = &image;
		RenderModel(renderContext, true);
		image.flip_vertically();
		image.write_tga_file("output_wireframe.tga");
	}

	while (!g_globals.bQuit)
	{
		MSG message;
		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		InvalidateRect(hwnd, nullptr, FALSE);
		UpdateWindow(hwnd);
	}

	delete g_renderTargets;
	return 0;
}