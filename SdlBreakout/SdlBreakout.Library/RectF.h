#include "Vector2.h"

#pragma once
class RectF
{
public:
	static RectF FromCentre(Vector2F centre, float width, float height)
	{
		return RectF(centre.x - width / 2, centre.y - height / 2, width, height);
	}

	RectF() = default;

	RectF(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{
	}

	RectF(Vector2F position, Vector2F size) : RectF(position.x, position.y, size.x, size.y)
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

	Vector2F TopLeft() const
	{
		return Vector2F(x, y);
	}

	Vector2F TopRight() const
	{
		return Vector2F(x + width, y);
	}

	Vector2F BottomLeft() const
	{
		return Vector2F(x, y + height);
	}

	Vector2F BottomRight() const
	{
		return Vector2F(x + width, y + height);
	}

	Vector2F Centre() const
	{
		return Vector2F(x + width / 2, y + height / 2);
	}

	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	void SetPosition(Vector2F position)
	{
		x = position.x;
		y = position.y;
	}

	Vector2F GetSize() const
	{
		return Vector2F(width, height);
	}

	bool Contains(const Vector2F& point) const
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

