#pragma once

#include "Graphics/Color.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Types.h"

namespace UI
{
	class Control
	{
	public:
		Control(const Graphics::Rectangle& bounds) : Bounds(bounds), Foreground(Graphics::Colors::Red), Background(Graphics::Colors::White)
		{

		}

		virtual void Draw(Graphics::FrameBuffer& frame) = 0;
		virtual void Update(const milli_t deltaTime) = 0;

		Graphics::Rectangle Bounds;
		Graphics::Color Foreground;
		Graphics::Color Background;
	};
}
