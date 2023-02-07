#pragma once

#include "UI/Control.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

#include <string>

namespace UI
{
	class Label : public Control
	{
	public:
		Label(const std::string& text, const Graphics::Rectangle& bounds) :
			Control(bounds),
			Text(text)
		{

		}

		virtual void Draw(Graphics::FrameBuffer& frame) override
		{
			frame.DrawRectangle(Background, Bounds);
			frame.DrawText({ Bounds.X + 5, Bounds.Y + 5 }, Text.c_str(), Foreground);
		}

		virtual void Update(const milli_t deltaTime) override
		{
			
		}

		std::string Text;
	};
}