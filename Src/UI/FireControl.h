#pragma once

#include "Assert.h"
#include "UI/Control.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"

namespace UI
{
	//DOOM Fire implmentation
	//http://fabiensanglard.net/doom_fire_psx/
	class FireControl : public Control
	{
	public:
		FireControl(const Graphics::Rectangle& bounds) :
			Control(bounds),
			m_indexes()
		{
			Assert(bounds.Height % PixelSize == 0);
			Assert(bounds.Width % PixelSize == 0);
			
			Initialize();
		}

		void Update()
		{
			for (size_t x = 0; x < Bounds.Width; x++)
					for (size_t y = 1; y < Bounds.Height; y++)
						SpreadFire({ x, y });
		}

		void Draw(Graphics::FrameBuffer &frameBuffer);

	private:
		static constexpr size_t PixelSize = 2;
		static constexpr size_t FireColorsCount = 37;
		static constexpr std::array<Graphics::Color, FireColorsCount> FireColors = {{
			{0x07, 0x07, 0x07, 0x00},
			{0x07, 0x07, 0x1F, 0x00},
			{0x07, 0x0F, 0x2F, 0x00},
			{0x07, 0x0F, 0x47, 0x00},
			{0x07, 0x17, 0x57, 0x00},
			{0x07, 0x1F, 0x67, 0x00},
			{0x07, 0x1F, 0x77, 0x00},
			{0x07, 0x27, 0x8F, 0x00},
			{0x07, 0x2F, 0x9F, 0x00},
			{0x07, 0x3F, 0xAF, 0x00},
			{0x07, 0x47, 0xBF, 0x00},
			{0x07, 0x47, 0xC7, 0x00},
			{0x07, 0x4F, 0xDF, 0x00},
			{0x07, 0x57, 0xDF, 0x00},
			{0x07, 0x57, 0xDF, 0x00},
			{0x07, 0x5F, 0xD7, 0x00},
			{0x07, 0x5F, 0xD7, 0x00},
			{0x0F, 0x67, 0xD7, 0x00},
			{0x0F, 0x6F, 0xCF, 0x00},
			{0x0F, 0x77, 0xCF, 0x00},
			{0x0F, 0x7F, 0xCF, 0x00},
			{0x17, 0x87, 0xCF, 0x00},
			{0x17, 0x87, 0xC7, 0x00},
			{0x17, 0x8F, 0xC7, 0x00},
			{0x1F, 0x97, 0xC7, 0x00},
			{0x1F, 0x9F, 0xBF, 0x00},
			{0x1F, 0x9F, 0xBF, 0x00},
			{0x27, 0xA7, 0xBF, 0x00},
			{0x27, 0xA7, 0xBF, 0x00},
			{0x2F, 0xAF, 0xBF, 0x00},
			{0x2F, 0xAF, 0xB7, 0x00},
			{0x2F, 0xB7, 0xB7, 0x00},
			{0x37, 0xB7, 0xB7, 0x00},
			{0x6F, 0xCF, 0xCF, 0x00},
			{0x9F, 0xDF, 0xDF, 0x00},
			{0xC7, 0xEF, 0xEF, 0x00},
			{0xFF, 0xFF, 0xFF, 0x00},
		}};

		void Initialize()
		{
			//Allocate index array
			m_indexes = new size_t[Bounds.Height * Bounds.Width];

			//Set bottom of indexes to highest index
			const size_t bottomY = Bounds.Height - 1;
			for (size_t x = 0; x < Bounds.Width; x++)
				m_indexes[bottomY * Bounds.Width + x] = FireColorsCount - 1;
		}

		void SpreadFire(Graphics::Point2D point);

		//Two dimensional array containing indexes into the FireColors array
		size_t *m_indexes;
	};
}