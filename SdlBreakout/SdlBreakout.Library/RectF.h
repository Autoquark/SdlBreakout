#include "Vector2.h"

#pragma once
class RectF
{
public:
	static RectF FromCentre(Vector2 centre, float width, float height)
	{
		return RectF(centre.x - width / 2, centre.y - height / 2, width, height);
	}

	RectF() = default;

	RectF(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{
	}

	float Top() const
	{
		return y;
	}

	float Bottom() const
	{
		return y + height;
	}

	float Left() const
	{
		return x;
	}

	float Right() const
	{
		return x + width;
	}

	Vector2 TopLeft() const
	{
		return Vector2(x, y);
	}

	Vector2 TopRight() const
	{
		return Vector2(x + width, y);
	}

	Vector2 BottomLeft() const
	{
		return Vector2(x, y + height);
	}

	Vector2 BottomRight() const
	{
		return Vector2(x + width, y + height);
	}

	Vector2 Centre() const
	{
		return Vector2(x + width / 2, y + height / 2);
	}

	bool Contains(const Vector2& point) const
	{
		return point.x >= x
			&& point.x <= x + width
			&& point.y >= y
			&& point.y <= y + height;
	}

	bool Overlaps(const RectF& other) const
	{
		return Right() >= other.Left()
			|| other.Right() >= Left()
			|| Top() >= other.Bottom()
			|| other.Top() >= Bottom();
	}

	float x, y, width, height;
};

