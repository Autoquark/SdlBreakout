#pragma once

#include "stdafx.h"

#include "Shape.h"
#include "AxisAlignedRectF.h"

#include <vector>

class CompoundShape : public Shape
{
public:
	CompoundShape(std::vector<Shape*> shapes) : shapes(shapes)
	{
	}

	[[nodiscard]]
	std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	[[nodiscard]]
	std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;

	void Translate(Vector2F amount) override;
	// Centres this shape's axis aligned bounding box on the given point
	void SetCentre(Vector2F position) override;

	[[nodiscard]]
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;

	std::vector<Shape*> shapes;
};

