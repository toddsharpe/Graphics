#pragma once

#include "Graphics/Color.h"
#include "Graphics/FrameBuffer.h"


namespace UI
{
	class Control
	{
	public:
		Control(const Graphics::Rectangle& bounds) : Bounds(bounds), Foreground(Graphics::Colors::Red), Background(Graphics::Colors::White)
		{

		}

		virtual void Draw(Graphics::FrameBuffer& frame) = 0;

		Graphics::Rectangle Bounds;
		Graphics::Color Foreground;
		Graphics::Color Background;
	};
}
