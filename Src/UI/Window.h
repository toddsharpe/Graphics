#pragma once

#include "Graphics/Color.h"
#include "Graphics/Types.h"
#include "UI/Control.h"

#include <string>
#include <vector>

namespace UI
{
	using namespace Graphics;

	class Window
	{
	private:
		static constexpr Color BorderColor = Colors::Blue;

	public:
		Window(const std::string& title) :
			Background(Colors::White),
			Children(),
			m_title(title)
		{

		}

		void Draw(FrameBuffer& frame)
		{
			//Clear frame
			frame.FillScreen(Background);
			
			//Draw border
			frame.DrawFrameBorder(BorderColor, 3);
			frame.DrawText({ 9, 9 }, m_title, BorderColor);
			frame.DrawRectangle(BorderColor, { 0, 20, frame.GetWidth(), 3 });

			//Draw children
			for (const auto& child : Children)
			{
				child->Draw(frame);
			}
		}

		void Update(const milli_t deltaTime)
		{
			for (const auto& child : Children)
			{
				child->Update(deltaTime);
			}
		}

		Color Background;
		std::vector<Control*> Children;

	private:
		std::string m_title;
	};
}
