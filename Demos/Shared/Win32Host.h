#pragma once

#include "Graphics/FrameBuffer.h"

#include <Windows.h>

struct DPad
{
	bool UpPressed;
	bool DownPressed;
	bool LeftPressed;
	bool RightPressed;
};

class Win32Host
{
public:
	static LRESULT CALLBACK Callback(void* arg, HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

	Win32Host(Graphics::FrameBuffer& frameBuffer, const size_t width, const size_t height);

	void Init(HINSTANCE instance, WNDPROC windowProc);

	void Start();
	void Dispatch();
	void GetInput(DPad& dpad);
	void DisplayBuffer();
	bool IsRunning();

	void Run();
	LRESULT CALLBACK Callback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

private:

	Graphics::FrameBuffer& m_frameBuffer;
	const size_t m_width;
	const size_t m_height;
	bool m_running;
	HDC m_deviceContext;
};

