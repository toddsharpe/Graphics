#pragma once

#include <cstddef>

namespace Graphics
{
	struct Point2D
	{
		size_t X;
		size_t Y;
	};

	struct Vector2i
	{
		int X;
		int Y;
	};

	struct Vector2f
	{
		float X;
		float Y;
	};

	struct Rectangle
	{
		size_t X;
		size_t Y;
		size_t Width;
		size_t Height;

		bool Contains(const Point2D& p) const
		{
			if (p.X < X || p.X > (X + Width))
				return false;

			if (p.Y < Y || p.Y >(Y + Height))
				return false;

			return true;
		}
	};
}