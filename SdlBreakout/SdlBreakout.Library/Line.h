#pragma once

#include "Shape.h"

class Line : public Shape
{
public:
	Line(Vector2F start, Vector2F end) : start(start), end(end) {}

	std::unique_ptr<Shape> Clone() const override
	{
		return std::make_unique<Line>(*this);
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

	using Shape::Translate;
	void Translate(Vector2F amount) override;
	void SetCentre(Vector2F position) override;
	// Returns a vector representing the displacement from the start to the end of this line
	Vector2F GetOffset() const
	{
		return end - start;
	}
	float Length() const
	{
		return GetOffset().Magnitude();
	}
	Vector2F GetPoint(float proportionalPosition) const
	{
		return Vector2F::LinearInterpolate(start, end, proportionalPosition);
	}

	[[nodiscard]]
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;

	Vector2F start;
	Vector2F end;

};

