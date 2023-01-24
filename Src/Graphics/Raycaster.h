#pragma once

#include "Graphics/Color.h"
#include "Graphics/FrameBuffer.h"

#include <cstddef>

//https://lodev.org/cgtutor/raycasting.html

namespace Graphics
{
	template <size_t Width, size_t Height>
	class Raycaster
	{
	public:
		Raycaster(FrameBuffer &frame) : m_frame(frame)
		{
		}

		void Render(const int (&worldMap)[Width][Height], const float posX, const float posY, const float dirX, const float dirY, const float planeX, const float planeY)
		{
			m_frame.FillScreen(Colors::Black);
			
			size_t w = m_frame.GetWidth();
			size_t h = m_frame.GetHeight();
			for (size_t x = 0; x < w; x++)
			{
				// calculate ray position and direction
				double cameraX = 2 * x / (double)w - 1; // x-coordinate in camera space
				double rayDirX = dirX + planeX * cameraX;
				double rayDirY = dirY + planeY * cameraX;

				// which box of the map we're in
				int mapX = int(posX);
				int mapY = int(posY);

				// length of ray from current position to next x or y-side
				double sideDistX;
				double sideDistY;

				// length of ray from one x or y-side to next x or y-side
				// these are derived as:
				// deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
				// deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
				// which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
				// where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
				// unlike (dirX, dirY) is not 1, however this does not matter, only the
				// ratio between deltaDistX and deltaDistY matters, due to the way the DDA
				// stepping further below works. So the values can be computed as below.
				//  Division through zero is prevented, even though technically that's not
				//  needed in C++ with IEEE 754 floating point values.
				double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
				double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

				double perpWallDist;

				// what direction to step in x or y-direction (either +1 or -1)
				int stepX;
				int stepY;

				int hit = 0; // was there a wall hit?
				int side;	 // was a NS or a EW wall hit?
				// calculate step and initial sideDist
				if (rayDirX < 0)
				{
					stepX = -1;
					sideDistX = (posX - mapX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (mapX + 1.0 - posX) * deltaDistX;
				}
				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = (posY - mapY) * deltaDistY;
				}
				else
				{
					stepY = 1;
					sideDistY = (mapY + 1.0 - posY) * deltaDistY;
				}
				// perform DDA
				while (hit == 0)
				{
					// jump to next map square, either in x-direction, or in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}
					// Check if ray has hit a wall
					if (worldMap[mapX][mapY] > 0)
						hit = 1;
				}
				// Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
				// hit to the camera plane. Euclidean to center camera point would give fisheye effect!
				// This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
				// for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
				// because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
				// steps, but we subtract deltaDist once because one step more into the wall was taken above.
				if (side == 0)
					perpWallDist = (sideDistX - deltaDistX);
				else
					perpWallDist = (sideDistY - deltaDistY);

				// Calculate height of line to draw on screen
				int lineHeight = (int)(h / perpWallDist);

				// calculate lowest and highest pixel to fill in current stripe
				int drawStart = -lineHeight / 2 + h / 2;
				if (drawStart < 0)
					drawStart = 0;
				int drawEnd = lineHeight / 2 + h / 2;
				if (drawEnd >= h)
					drawEnd = h - 1;

				// choose wall color
				Color color;
				switch (worldMap[mapX][mapY])
				{
				case 1:
					color = Colors::Red;
					break; // red
				case 2:
					color = Colors::Green;
					break; // green
				case 3:
					color = Colors::Blue;
					break; // blue
				case 4:
					color = Colors::White;
					break; // white
				default:
					color = Colors::Yellow;
					break; // yellow
				}

				// give x and y sides different brightness
				if (side == 1)
				{
					color = color / 2;
				}

				// draw the pixels of the stripe as a vertical line
				m_frame.DrawVerticalLine(color, x, drawStart, drawEnd);
			}
		}

	private:
		FrameBuffer &m_frame;
	};
}