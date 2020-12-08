#pragma once

#include "stdafx.h"

#include "Shape.h"
#include "AxisAlignedRectF.h"

#include <vector>
#include <algorithm>
#include <iterator>

class CompoundShape : public Shape
{
public:
	explicit CompoundShape(const std::vector<std::unique_ptr<Shape>>& shapes)
	{
		std::transform(shapes.cbegin(),
			shapes.cend(),
			std::back_inserter(this->shapes),
			[](auto& shape) { return std::move(shape->Clone()); });
	}

	CompoundShape(const CompoundShape& compoundShape)
	{
		std::transform(compoundShape.shapes.cbegin(),
			compoundShape.shapes.cend(),
			std::back_inserter(this->shapes),
			[](auto& shape) { return std::move(shape->Clone()); });
	}

	std::unique_ptr<Shape> Clone() const override
	{
		return std::make_unique<CompoundShape>(*this);
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

	std::vector<std::unique_ptr<Shape>> shapes;
};

