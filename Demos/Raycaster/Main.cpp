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
//WindowsFrameBuffer frameBuffer(480, 272);
Win32Host host(frameBuffer, frameBuffer.GetWidth()* scale, frameBuffer.GetHeight()* scale);

LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	return host.Callback(Window, Message, WParam, LParam);
}

#define mapWidth 24
#define mapHeight 24
int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	//Initialize host
	host.Init(hInstance, Win32MainWindowCallback);

	//Raycaster
	Graphics::Raycaster<24, 24> raycaster(frameBuffer);
	float posX = 22, posY = 12;  //x and y start position
	float dirX = -1, dirY = 0; //initial direction vector
	float planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

	float moveSpeed = 5.0 / 30.0;
	float rotSpeed = 3.0 / 30.0;

	DPad dpad = {};

	//Run host
	host.Start();
	while (host.IsRunning())
	{
		//Dispatch host
		host.Dispatch();
		host.GetInput(dpad);

		if (dpad.UpPressed)
		{
			if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
			if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
		}
		//move backwards if no wall behind you
		if (dpad.DownPressed)
		{
			if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
			if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
		}
		//rotate to the right
		if (dpad.RightPressed)
		{
			//both camera direction and camera plane must be rotated
			float oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			float oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}
		//rotate to the left
		if (dpad.LeftPressed)
		{
			//both camera direction and camera plane must be rotated
			float oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			float oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}


		//Draw
		raycaster.Render(worldMap, posX, posY, dirX, dirY, planeX, planeY);
		host.DisplayBuffer();

		Sleep(1000.0 / 30);
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
