#pragma once

#include "stdafx.h"
#include "Vector2.h"
#include "Shape.h"

class CircleF : public Shape
{
public:
	CircleF(Vector2F centre, float radius) : centre(centre), radius(radius){}
	CircleF(float x, float y, float radius) : CircleF(Vector2F(x, y), radius){}

	std::unique_ptr<Shape> Clone() const override
	{
		return std::make_unique<CircleF>(*this);
	}

	Vector2F centre;
	float radius;

	[[nodiscard]]
	std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;

	void Translate(Vector2F amount) override
	{
		centre += amount;
	}

	void SetCentre(Vector2F position) override
	{
		centre = position;
	}

	[[nodiscard]]
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;
};

