#pragma once

#include "Graphics/Color.h"
#include "Graphics/Types.h"
#include "Graphics/FrameBuffer.h"
#include "UI/Types.h"

#include <cstddef>

//https://lodev.org/cgtutor/raycasting.html

namespace Graphics
{
	template <size_t Width, size_t Height>
	class Raycaster
	{
	public:
		Raycaster(const int(&worldMap)[Width][Height]) : 
			m_worldMap(worldMap),
			m_pos({22, 12}),
			m_dir({ -1, 0 }),
			m_plane({0, 0.66})
		{

		}

		void Update(UI::DPad& input, const size_t ms)
		{
			float moveSpeed = MOVE_SPEED * ms / 1000;
			float rotSpeed = ROTATION_SPEED * ms / 1000;

			if (input.UpPressed)
			{
				if (m_worldMap[int(m_pos.X + m_dir.X * moveSpeed)][int(m_pos.Y)] == false)
					m_pos.X += m_dir.X * moveSpeed;
				if (m_worldMap[int(m_pos.X)][int(m_pos.Y + m_dir.Y * moveSpeed)] == false)
					m_pos.Y += m_dir.Y * moveSpeed;
			}

			//move backwards if no wall behind you
			if (input.DownPressed)
			{
				if (m_worldMap[int(m_pos.X - m_dir.X * moveSpeed)][int(m_pos.Y)] == false)
					m_pos.X -= m_dir.X * moveSpeed;
				if (m_worldMap[int(m_pos.X)][int(m_pos.Y - m_dir.Y * moveSpeed)] == false)
					m_pos.Y -= m_dir.Y * moveSpeed;
			}

			//rotate to the right
			if (input.RightPressed)
			{
				//both camera direction and camera plane must be rotated
				float oldDirX = m_dir.X;
				m_dir.X = m_dir.X * cos(-rotSpeed) - m_dir.Y * sin(-rotSpeed);
				m_dir.Y = oldDirX * sin(-rotSpeed) + m_dir.Y * cos(-rotSpeed);

				float oldPlaneX = m_plane.X;
				m_plane.X = m_plane.X * cos(-rotSpeed) - m_plane.Y * sin(-rotSpeed);
				m_plane.Y = oldPlaneX * sin(-rotSpeed) + m_plane.Y * cos(-rotSpeed);
			}

			//rotate to the left
			if (input.LeftPressed)
			{
				//both camera direction and camera plane must be rotated
				float oldDirX = m_dir.X;
				m_dir.X = m_dir.X * cos(rotSpeed) - m_dir.Y * sin(rotSpeed);
				m_dir.Y = oldDirX * sin(rotSpeed) + m_dir.Y * cos(rotSpeed);

				float oldPlaneX = m_plane.X;
				m_plane.X = m_plane.X * cos(rotSpeed) - m_plane.Y * sin(rotSpeed);
				m_plane.Y = oldPlaneX * sin(rotSpeed) + m_plane.Y * cos(rotSpeed);
			}
		}

		void Render(FrameBuffer& frame, const size_t ms)
		{
			//Clear frame
			frame.FillScreen(Colors::Black);
			
			//Scan left to right
			for (size_t x = 0; x < frame.GetWidth(); x++)
			{
				// calculate ray position and direction
				float cameraX = 2 * x / (float)frame.GetWidth() - 1; // x-coordinate in camera space
				Vector2f ray = { m_dir.X + m_plane.X * cameraX , m_dir.Y + m_plane.Y * cameraX };

				// which box of the map we're in
				Vector2i map = { int(m_pos.X), int(m_pos.Y) };

				// length of ray from current position to next x or y-side
				Vector2f sideDist = {};

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
				Vector2f deltaDist =
				{
					(ray.X == 0) ? 1e30 : std::abs(1 / ray.X),
					(ray.Y == 0) ? 1e30 : std::abs(1 / ray.Y)
				};

				float perpWallDist;

				// what direction to step in x or y-direction (either +1 or -1)
				Vector2i step = {};

				int hit = 0; // was there a wall hit?
				int side;	 // was a NS or a EW wall hit?

				// calculate step and initial sideDist
				if (ray.X < 0)
				{
					step.X = -1;
					sideDist.X = (m_pos.X - map.X) * deltaDist.X;
				}
				else
				{
					step.X = 1;
					sideDist.X = (map.X + 1.0 - m_pos.X) * deltaDist.X;
				}

				if (ray.Y < 0)
				{
					step.Y = -1;
					sideDist.Y = (m_pos.Y - map.Y) * deltaDist.Y;
				}
				else
				{
					step.Y = 1;
					sideDist.Y = (map.Y + 1.0 - m_pos.Y) * deltaDist.Y;
				}

				// perform DDA
				while (hit == 0)
				{
					// jump to next map square, either in x-direction, or in y-direction
					if (sideDist.X < sideDist.Y)
					{
						sideDist.X += deltaDist.X;
						map.X += step.X;
						side = 0;
					}
					else
					{
						sideDist.Y += deltaDist.Y;
						map.Y += step.Y;
						side = 1;
					}
					// Check if ray has hit a wall
					if (m_worldMap[map.X][map.Y] > 0)
						hit = 1;
				}

				// Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
				// hit to the camera plane. Euclidean to center camera point would give fisheye effect!
				// This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
				// for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
				// because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
				// steps, but we subtract deltaDist once because one step more into the wall was taken above.
				if (side == 0)
					perpWallDist = (sideDist.X - deltaDist.X);
				else
					perpWallDist = (sideDist.Y - deltaDist.Y);

				// Calculate height of line to draw on screen
				int lineHeight = (int)(frame.GetHeight() / perpWallDist);

				// calculate lowest and highest pixel to fill in current stripe
				int drawStart = -lineHeight / 2 + frame.GetHeight() / 2;
				if (drawStart < 0)
					drawStart = 0;
				int drawEnd = lineHeight / 2 + frame.GetHeight() / 2;
				if (drawEnd >= frame.GetHeight())
					drawEnd = frame.GetHeight() - 1;

				// choose wall color
				Color color;
				switch (m_worldMap[map.X][map.Y])
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
				frame.DrawVerticalLine(color, x, drawStart, drawEnd);
			}

			//Write FPS
			size_t fps = 1000 / ms;
			frame.DrawPrintf({ 0, 0 }, Colors::Blue, "FPS: %d", fps);
		}

	private:
		static constexpr float MOVE_SPEED = 5.0;
		static constexpr float ROTATION_SPEED = 3.0;

		const int(&m_worldMap)[Width][Height];
		Vector2f m_pos;
		Vector2f m_dir;
		Vector2f m_plane;
	};
}