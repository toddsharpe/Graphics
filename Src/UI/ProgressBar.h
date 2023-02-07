#pragma once

#include "UI/Control.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

#include <string>

namespace UI
{
	class ProgressBar : public Control
	{
	public:
		ProgressBar(const std::string& text, const Graphics::Rectangle& bounds) :
			Control(bounds),
			Value(0),
			MaxValue(10),
			InactiveColor(Colors::White),
			Text(text)
		{

		}

		virtual void Draw(Graphics::FrameBuffer& frame) override
		{
			const size_t activeWidth = Bounds.Width * Value / MaxValue;
			const size_t inactiveWidth = Bounds.Width - activeWidth;
			
			//Draw background
			frame.DrawRectangle(Background, Bounds);

			//Draw active
			frame.DrawRectangle(Foreground, { Bounds.X, Bounds.Y, activeWidth, Bounds.Height });

			//Draw inactive
			frame.DrawRectangle(InactiveColor, { Bounds.X + activeWidth, Bounds.Y, inactiveWidth, Bounds.Height });

			frame.DrawText({ Bounds.X, Bounds.Y + Bounds.Height + 5 }, Text.c_str(), Foreground);
		}

		virtual void Update(const milli_t deltaTime) override
		{
			
		}

		size_t Value;
		size_t MaxValue;
		Graphics::Color InactiveColor;
		std::string Text;
	};
}