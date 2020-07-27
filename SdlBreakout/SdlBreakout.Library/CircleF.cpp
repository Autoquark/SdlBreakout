#include "stdafx.h"
#include "CircleF.h"
#include "Point.h"

std::optional<Contact> CircleF::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return other.CastAgainstThis(*this, movement, internalityFilter);
}

std::optional<Contact> CircleF::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	auto centreToStart = other - centre;

	auto a = movement.DotProduct(movement);
	auto b = 2 * centreToStart.DotProduct(movement);
	auto c = centreToStart.DotProduct(centreToStart) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return std::nullopt;
	}

	discriminant = std::sqrt(discriminant);
	float t1 = (-b - discriminant) / (2 * a);
	float t2 = (-b + discriminant) / (2 * a);

	auto pointEndPosition = other + movement;

	if (t1 >= 0 && t1 <= 1 && internalityFilter != InternalityFilter::Internal)
	{
		// t1 is the intersection, and it's closer than t2
		// (since t1 uses -b - discriminant)
		// This must be an external hit
		auto point = Vector2F::LinearInterpolate(other, pointEndPosition, t1);
		return Contact((point - centre).Normalised(), point, true, (point - other).Magnitude());
	}
	else if (t2 >= 0 && t2 <= 1 && internalityFilter != InternalityFilter::External)
	{
		// Internal hit
		auto point = Vector2F::LinearInterpolate(other, pointEndPosition, t2);
		return Contact((centre - point).Normalised(), point, false, (point - other).Magnitude());
	}

	return std::nullopt;
}

std::optional<Contact> CircleF::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}
