#pragma once

#include "Shape.h"

class AxisAlignedRectF;
class CircleF;
class Line;

class Point : public Shape
{
public:
	Point(const Vector2F& position) : position(position)
	{ }

	operator Vector2F() const
	{
		return position;
	}

	Vector2F position;

	virtual std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;

	void Translate(Vector2F amount) override;
	void SetCentre(Vector2F position) override;
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;
};

