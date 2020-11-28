#pragma once

#include "Shape.h"

class AxisAlignedRectF;
class CircleF;
class Line;

// Represents a point with a location but no size.
// Collisions with a Point are always considered to be with the exterior of the Point
class Point : public Shape
{
public:
	explicit Point(const Vector2F& position) : position(position)
	{ }

	operator Vector2F() const
	{
		return position;
	}

	Vector2F position;

	std::unique_ptr<Shape> Clone() const override
	{
		return std::make_unique<Point>(*this);
	}

	[[nodiscard]]
	virtual std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;

	void Translate(Vector2F amount) override;
	void SetCentre(Vector2F position) override;

	[[nodiscard]]
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;
};

