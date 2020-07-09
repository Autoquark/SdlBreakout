#include "Vector2.h"

#pragma once
class RectF
{
public:
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

	bool Contains(const Vector2& point) const
	{
		return point.x >= x
			&& point.x <= x + width
			&& point.y >= y
			&& point.y <= y + height;
	}

	float x, y, width, height;
};

