#include "Vector2.h"
#include "Shape.h"

class CircleF;
class Point;
class Line;

#pragma once
// Represents an axis-aligned rectangle
class AxisAlignedRectF : public Shape
{
public:
	std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	void Translate(Vector2F amount) override;
	void Union(const Shape& other);
	void Union(const AxisAlignedRectF& other);

	static AxisAlignedRectF FromCentre(Vector2F centre, float width, float height)
	{
		return AxisAlignedRectF(centre.x - width / 2, centre.y - height / 2, width, height);
	}

	static AxisAlignedRectF FromEdges(float leftX, float topY, float rightX, float bottomY)
	{
		return AxisAlignedRectF(leftX, topY, rightX - leftX, bottomY - topY);
	}

	AxisAlignedRectF(float x, float y, float width, float height) : AxisAlignedRectF(Vector2F(x, y), Vector2F(width, height))
	{
	}

	AxisAlignedRectF(Vector2F position, Vector2F size) : position(position), size(size)
	{
		assert(size.x >= 0);
		assert(size.y >= 0);
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

	/// <summary>
	/// Resizes the rectangle so that the left edge is at the given x value, without changing the other edge positions
	/// </summary>
	void SetLeft(float value)
	{
		size.x += (position.x - value);
		position.x = value;
	}

	/// <summary>
	/// Resizes the rectangle so that the right edge is at the given x value, without changing the other edge positions
	/// </summary>
	void SetRight(float value)
	{
		size.x = value - position.x;
	}

	/// <summary>
	/// Resizes the rectangle so that the top edge is at the given x value, without changing the other edge positions
	/// </summary>
	void SetTop(float value)
	{
		size.y += (position.y - value);
		position.y = value;
	}

	/// <summary>
	/// Resizes the rectangle so that the bottom edge is at the given x value, without changing the other edge positions
	/// </summary>
	void SetBottom(float value)
	{
		size.y = value - position.y;
	}

	/// <summary>
	/// Returns the top edge of the rectangle, defined such that a Contact with side = true indicates an external collision
	/// </summary>
	Line TopEdge() const;

	/// <summary>
	/// Returns the right edge of the rectangle, defined such that a Contact with side = true indicates an external collision
	/// </summary>
	Line RightEdge() const;

	/// <summary>
	/// Returns the bottom edge of the rectangle, defined such that a Contact with side = true indicates an external collision
	/// </summary>
	Line BottomEdge() const;

	/// <summary>
	/// Returns the left edge of the rectangle, defined such that a Contact with side = true indicates an external collision
	/// </summary>
	Line LeftEdge() const;

	void Rotate90()
	{
		auto centre = Centre();
		position -= centre;
		std::swap(position.x, position.y);
		std::swap(size.x, size.y);
		position += centre;
	}

	AxisAlignedRectF Rotated90()
	{
		auto clone = AxisAlignedRectF(*this);
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

	bool Overlaps(const AxisAlignedRectF& other) const
	{
		return Right() >= other.Left()
			|| other.Right() >= Left()
			|| Top() >= other.Bottom()
			|| other.Top() >= Bottom();
	}

	// Position of the top left of the rectangle
	Vector2F position;
	// Size of the rectangle
	Vector2F size;

	void SetCentre(Vector2F position) override
	{
		this->position = position - size / 2;
	}

	AxisAlignedRectF GetAxisAlignedBoundingBox() const override
	{
		return *this;
	}
};

