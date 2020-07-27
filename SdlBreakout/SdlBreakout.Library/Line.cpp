#include "stdafx.h"
#include "Line.h"
#include "Point.h"
#include "GeneralFormLine.h"
#include "CircleF.h"
#include "AxisAlignedRectF.h"

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
	// Can currently only handle horizontal or vertical lines
	if (start.x != end.x && start.y != end.y)
	{
		throw std::exception();
	}

	auto circleEndCentre = other.centre + movement;
	auto linePerpendicular = (end - start).Rotated(90)
		.Normalised();
	auto rect = AxisAlignedRectF::FromCentre(Vector2F::LinearInterpolate(start, end, 0.5f), Vector2F::DistanceBetween(start, end), other.radius * 2);
	if (start.x == end.x)
	{
		rect.Rotate90();
	}

	auto circleCentre = Point(other.centre);
	std::vector<std::optional<Contact>> contacts = {
		rect.CastAgainstThis(circleCentre, movement),
		CircleF(start, other.radius).CastAgainstThis(circleCentre, movement),
		CircleF(end, other.radius).CastAgainstThis(circleCentre, movement),
	};

	auto nullable = FindClosestCollision(contacts);
	if (!nullable.has_value())
	{
		return std::nullopt;
	}
	auto contact = *nullable;

	auto normal = end - start;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(circleEndCentre - other.centre);

	return Contact(contact.normal, contact.point + other.radius * movement.Normalised(), dotProduct < 0, contact.distance, contact.point);
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
