#pragma once

#include "UI/Control.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

#include <string>

namespace UI
{
	using namespace Graphics;

	class ProgressBar : public Control
	{
	public:
		ProgressBar(const std::string& text, const Rectangle& bounds) :
			Control(),
			Bounds(bounds),
			Text(text)
		{

		}

		virtual void Draw(FrameBuffer& frame) override
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

		Color InactiveColor;
		size_t Value;
		size_t MaxValue;
		Rectangle Bounds;
		std::string Text;
	};
}