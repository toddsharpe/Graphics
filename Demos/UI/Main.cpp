//#include "WindowsFrameBuffer.h"
#include "Graphics/StaticFrameBuffer.h"
#include "Graphics/Raycaster.h"
#include "UI/Window.h"
#include "UI/Label.h"
#include "UI/ProgressBar.h"

#include <Assert.h>
#include <cstdint>
#include <stdio.h>
#include "Win32Host.h"

constexpr size_t scale = 2;
Graphics::StaticFrameBuffer<480, 272> frameBuffer;

Win32Host host(frameBuffer, frameBuffer.GetWidth()* scale, frameBuffer.GetHeight()* scale);
UI::Window window("Graphics Dev");

LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	return host.Callback(Window, Message, WParam, LParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	//Initialize host
	host.Init(hInstance, Win32MainWindowCallback);

	//Populate window
	window.Background = Graphics::Colors::Black;

	UI::Label output("Output", { 5, 25, 0, 0 });
	window.Children.push_back(&output);
	output.Text = "Test!";

	UI::ProgressBar progressBar("Progress", { 25, 200, 400, 20 });
	progressBar.Foreground = Graphics::Colors::Red;
	progressBar.Background = Graphics::Colors::Black;
	progressBar.InactiveColor = Graphics::Colors::White;
	progressBar.Value = 0;
	progressBar.MaxValue = 100;
	window.Children.push_back(&progressBar);

	//Run host
	host.Start();
	while (host.IsRunning())
	{
		//Dispatch host
		host.Dispatch();

		//Update
		progressBar.Value += 1;
		if (progressBar.Value > progressBar.MaxValue)
			progressBar.Value = 0;

		//Draw window
		window.Draw(frameBuffer);

		//Overlay a circle
		frameBuffer.DrawCircle(Graphics::Colors::Green, { 100, 100 }, 50);

		host.DisplayBuffer();
		Sleep(1000.0 / 10);
	}
}

void Bugcheck(const char* file, const char* line, const char* format, ...)
{
	printf("Bugcheck\r\n");
	printf("\r\n%s\r\n%s\r\n", file, line);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\r\n");
	va_end(args);

	DebugBreak();
	exit(-1);
}
