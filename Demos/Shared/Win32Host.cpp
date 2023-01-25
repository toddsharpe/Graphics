#include "Win32Host.h"
#include "Assert.h"

LRESULT Win32Host::Callback(void* arg, HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	return ((Win32Host*)arg)->Callback(Window, Message, WParam, LParam);
}

Win32Host::Win32Host(Graphics::FrameBuffer& frameBuffer, const size_t width, const size_t height) :
	m_frameBuffer(frameBuffer),
	m_width(width),
	m_height(height),
	m_running(),
	m_deviceContext()
{

}

void Win32Host::Init(HINSTANCE instance, WNDPROC windowProc)
{
	WNDCLASSA WindowClass = {};
	WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowClass.lpfnWndProc = windowProc;
	WindowClass.hInstance = instance;
	WindowClass.lpszClassName = "GraphicsDemoClass";

	//Register class
	Assert(RegisterClassA(&WindowClass));

	//Resize to right client rect
	RECT desired = {};
	desired.left = 0;
	desired.top = 0;
	desired.right = m_width;
	desired.bottom = m_height;
	Assert(AdjustWindowRect(&desired, WS_OVERLAPPEDWINDOW, false));

	//Create window
	HWND Window = CreateWindowEx(
		0,
		WindowClass.lpszClassName,
		"Graphics Demos",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(desired.right - desired.left),
		(desired.bottom - desired.top),
		NULL,
		NULL,
		instance,
		NULL
	);
	Assert(Window);

	//Get device context (CS_OWNDC means we own it)
	m_deviceContext = GetDC(Window);
}

void Win32Host::Start()
{
	m_running = true;
}

void Win32Host::Dispatch()
{
	MSG msg = { };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Win32Host::GetInput(UI::DPad& dpad)
{
	const SHORT pressed = (1 << 15);
	dpad.LeftPressed = GetKeyState(VK_LEFT) & pressed;
	dpad.RightPressed = GetKeyState(VK_RIGHT) & pressed;
	dpad.UpPressed = GetKeyState(VK_UP) & pressed;
	dpad.DownPressed = GetKeyState(VK_DOWN) & pressed;
}

bool Win32Host::IsRunning()
{
	return m_running;
}

void Win32Host::Run()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Win32Host::Callback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch (Message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT Paint;
		HDC DeviceContext = BeginPaint(Window, &Paint);
		Assert(DeviceContext == m_deviceContext);

		RECT ClientRect;
		GetClientRect(Window, &ClientRect);
		int width = ClientRect.right - ClientRect.left;
		int height = ClientRect.bottom - ClientRect.top;

		DisplayBuffer();
		EndPaint(Window, &Paint);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_QUIT:
		m_running = false;
		break;

	default:
		return DefWindowProc(Window, Message, WParam, LParam);
	}
	return 0;
}

void Win32Host::DisplayBuffer()
{
	BITMAPINFO Info;
	Info.bmiHeader.biSize = sizeof(Info.bmiHeader);
	Info.bmiHeader.biWidth = m_frameBuffer.GetWidth();
	Info.bmiHeader.biHeight = -(int)(m_frameBuffer.GetHeight());
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = 8 * m_frameBuffer.PixelSize();
	Info.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(
		m_deviceContext,
		0, 0, m_width, m_height,
		0, 0, m_frameBuffer.GetWidth(), m_frameBuffer.GetHeight(),
		m_frameBuffer.GetBuffer(),
		&Info,
		DIB_RGB_COLORS, SRCCOPY
	);
}
