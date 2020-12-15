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

	// Gets a value indicating which side of this line the given point lies on. A positive value indicates that the point is to the right, a negative value to the left and zero that the point lies
	// on the line (if the line were extended to infinity)
	float SidednessValue(Vector2F point) const
	{
		auto offset = GetOffset();
		// Negate the result so that positive corresponds to a true Contact::movingSide / Contact::stationarySide value
		return -((point.x - start.x) * offset.y - (point.y - start.y) * offset.x);
	}

	[[nodiscard]]
	AxisAlignedRectF GetAxisAlignedBoundingBox() const override;

	Vector2F start;
	Vector2F end;

};

