#include "Vector2.h"

#pragma once
// Represents an axis-aligned rectangle
class RectF
{
public:
	static RectF FromCentre(Vector2F centre, float width, float height)
	{
		return RectF(centre.x - width / 2, centre.y - height / 2, width, height);
	}

	RectF() = default;

	RectF(float x, float y, float width, float height) : RectF(Vector2F(x, y), Vector2F(width, height))
	{
	}

	RectF(Vector2F position, Vector2F size) : position(position), size(size)
	{
	}

	float Top() const
	{
		return position.y;
	}

	float Bottom() const
	{
		return position.y + size.y;
	}

	float Left() const
	{
		return position.x;
	}

	float Right() const
	{
		return position.x + size.x;
	}

	Vector2F TopLeft() const
	{
		return position;
	}

	Vector2F TopRight() const
	{
		return position + Vector2F(size.x, 0);
	}

	Vector2F BottomLeft() const
	{
		return position + Vector2F(0, size.y);
	}

	Vector2F BottomRight() const
	{
		return position + size;
	}

	Vector2F Centre() const
	{
		return position + size / 2;
	}

	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	void SetPosition(Vector2F position)
	{
		this->position = position;
	}

	void Rotate90()
	{
		auto centre = Centre();
		position -= centre;
		std::swap(position.x, position.y);
		std::swap(size.x, size.y);
		position += centre;
	}

	RectF Rotated90()
	{
		auto clone = RectF(*this);
		clone.Rotate90();
		return clone;
	}

	bool Contains(const Vector2F& point) const
	{
		return point.x >= position.x
			&& point.x <= position.x + size.x
			&& point.y >= position.y
			&& point.y <= position.y + size.y;
	}

	bool Overlaps(const RectF& other) const
	{
		return Right() >= other.Left()
			|| other.Right() >= Left()
			|| Top() >= other.Bottom()
			|| other.Top() >= Bottom();
	}

	Vector2F position, size;
};

