#include "stdafx.h"
#include "Line.h"
#include "Point.h"
#include "GeneralFormLine.h"

std::optional<Contact> Line::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Line::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Line::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Line::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	auto pointTrajectory = GeneralFormLine(other, other + movement);
	auto line = GeneralFormLine(start, end);

	auto denominator = line.xCoefficient * pointTrajectory.yCoefficient - pointTrajectory.xCoefficient * line.yCoefficient;

	float x;
	// Special case: two segments of the same line
	if (denominator == 0)
	{
		return std::nullopt;
	}
	else
	{
		x = (pointTrajectory.constant * line.yCoefficient - line.constant * pointTrajectory.yCoefficient)
			/ denominator;
	}

	float y;
	if (!line.TryYFromX(x, y))
	{
		pointTrajectory.TryYFromX(x, y);
	}

	// Need to check both x and y to handle vertical lines
	if (x < pointTrajectory.minX || x < line.minX || x > pointTrajectory.maxX || x > line.maxX || y < pointTrajectory.minY || y < line.minY || y > pointTrajectory.maxY || y > line.maxY)
	{
		return std::nullopt;
	}

	Vector2F contactPoint = { x, y };

	auto normal = end - start;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(movement);
	normal = normal * dotProduct;
	normal.Normalise();

	return Contact(-normal, contactPoint, dotProduct < 0, Vector2F::DistanceBetween(contactPoint, other));
}

std::optional<Contact> Line::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}
