#pragma once

#include "Assert.h"
#include "Graphics/Font.h"
#include "Graphics/Color.h"
#include "Graphics/Types.h"

#include <cstdarg>
#include <string>

namespace Graphics
{
	class FrameBuffer
	{
	public:
		virtual size_t GetHeight() const = 0;
		virtual size_t GetWidth() const = 0;
		virtual Color* GetBuffer() = 0;

		FrameBuffer() : m_font()
		{
			
		}

		void FillScreen(const Color color)
		{
			//this->DrawRectangle(color, {0, 0, GetWidth(), GetHeight()});
			SetPixels(GetBuffer(), color, GetWidth() * GetHeight());
		}

		void DrawVerticalLine(const Color color, const size_t x, const size_t y1, const size_t y2)
		{
			DrawRectangle(color, { x, y1, 1, (y2 - y1) });
		}

		void DrawHorizontalLine(const Color color, const size_t y, const size_t x1, const size_t x2)
		{
			DrawRectangle(color, { x1, y, (x2 - x1), 1 });
		}

		void DrawRectangle(const Color color, const Rectangle& region)
		{
			for (size_t y = 0; y < region.Height; y++)
			{
				if (y + region.Y >= GetHeight())
					break;

				//Write row
				Color* dst = GetBuffer() + (region.Y + y) * GetWidth() + region.X;
				const size_t count = std::min(region.Width, GetWidth() - region.X);
				SetPixels(dst, color, count);
			}
		}

		void DrawFrameBorder(const Color color, const size_t thickness)
		{
			//Top
			SetPixels(GetBuffer(), color, GetWidth() * thickness);

			//Bottom
			Color* dst = (Color*)GetBuffer() + (GetHeight() - thickness) * GetWidth();
			SetPixels(dst, color, GetWidth() * thickness);

			//Sides
			for (size_t y = thickness; y < GetHeight() - thickness; y++)
			{
				Color* row = (Color*)GetBuffer() + y * GetWidth();

				//Left side
				SetPixels(row, color, thickness);

				//Right side
				SetPixels(row + (GetWidth() - thickness), color, thickness);
			}
		}

		void DrawBorder(const Color color, const Rectangle& rect, const size_t thickness)
		{
			//Top
			DrawRectangle(color, { rect.X, rect.Y, rect.Width, thickness });

			//Left
			DrawRectangle(color, { rect.X, rect.Y, thickness, rect.Height });

			//Right
			DrawRectangle(color, { rect.X + rect.Width, rect.Y, thickness, rect.Height });

			//Bottom
			DrawRectangle(color, { rect.X, rect.Y + rect.Height, rect.Width, thickness });
		}

		void DrawText(const Point2D position, const std::string& text, const Color color)
		{
			Point2D pos = position;
			for (const char c : text)
			{
				if (c == '\n')
				{
					pos.Y += PixelScale;
					pos.X = position.X;
				}
				else if (c != '\r')
				{
					DrawCharacter(pos, c, color);
					pos.X += PixelScale;
				}
			}
		}

		void DrawPrintf(Point2D position, const Color color, const char* format, ...)
		{
			char buffer[255];

			va_list args;
			va_start(args, format);
			int retval = vsprintf(buffer, format, args);
			buffer[retval] = '\0';
			va_end(args);

			this->DrawText(position, std::string(buffer), color);
		}

		Color GetPixel(const Point2D& position)
		{
			Assert(position.X < GetWidth());
			Assert(position.Y < GetHeight());

			return GetBuffer()[position.Y * GetWidth() + position.X];
		}

		size_t Size() const
		{
			return GetHeight() * GetWidth() * sizeof(Color);
		}

		size_t PixelSize() const
		{
			return sizeof(Color);
		}

	private:
		static constexpr size_t PixelScale = 10;
		static constexpr size_t FontScale = 1;

		static void SetPixels(Color* _Dst, const Color _Val, const size_t _Size)
		{
			for (size_t i = 0; i < _Size; i++)
			{
				*_Dst = (Color)_Val;
				_Dst++;
			}
		}

		void DrawCharacter(const Point2D position, const char c, const Color color)
		{
			const char* map = m_font.GetCharacterMap(c);
			uint8_t size = 8; // TODO: get from font

			for (size_t y = 0; y < size; y++)
			{
				//Line is reversed
				char line = map[y];

				size_t x = 0;
				unsigned char mask = 0x80;
				while (mask > 0)
				{
					if ((line & mask) != 0)
					{
						Rectangle rect;
						rect.X = (position.X + (8 - x - 1)) * FontScale;
						rect.Y = (position.Y + y) * FontScale;
						rect.Width = FontScale;
						rect.Height = FontScale;
						DrawRectangle(color, rect);
					}

					mask = mask >> 1;
					x++;
				}
			}
		}

		const Font m_font;
	};
}