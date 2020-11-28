#include "stdafx.h"
#include "Line.h"
#include "Point.h"
#include "GeneralFormLine.h"
#include "CircleF.h"
#include "AxisAlignedRectF.h"

std::optional<Contact> Line::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

std::optional<Contact> Line::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

std::optional<Contact> Line::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Can currently only handle horizontal or vertical lines; Need rotated rectangle type for other cases
	if (start.x != end.x && start.y != end.y)
	{
		throw std::exception();
	}

	// Reduce the circle to a point, and expand the line in the two perpendicular directions to form a rectangle.
	// Also check against two circles placed at either end of the line with radius equal to the radius of the original circle.
	auto circleEndCentre = other.centre + movement;

	// Create a rectangle that's long in the horizontal axis
	auto rect = AxisAlignedRectF::FromCentre(Vector2F::LinearInterpolate(start, end, 0.5f), Vector2F::DistanceBetween(start, end), other.radius * 2);
	// If this is a vertical line, rotate the rectangle
	if (start.x == end.x)
	{
		rect.Rotate90();
	}

	auto circleCentre = Point(other.centre);
	std::optional<Contact> bestContact = rect.CastAgainstThis(circleCentre, movement);
	bestContact = ClosestContact(bestContact, CircleF(start, other.radius).CastAgainstThis(circleCentre, movement));
	bestContact = ClosestContact(bestContact, CircleF(end, other.radius).CastAgainstThis(circleCentre, movement));

	if (!bestContact.has_value())
	{
		return std::nullopt;
	}
	auto& contact = *bestContact;

	auto normal = end - start;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(circleEndCentre - other.centre);

	return Contact(contact.normal, contact.point + other.radius * movement.Normalised(), dotProduct < 0, true /*TODO: Case where circle envelops line*/,  contact.distance, contact.point);
}

std::optional<Contact> Line::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	auto pointTrajectory = GeneralFormLine(other, other + movement);
	auto line = GeneralFormLine(start, end);

	auto denominator = line.xCoefficient * pointTrajectory.yCoefficient - pointTrajectory.xCoefficient * line.yCoefficient;

	float x;
	// Special case: parallel lines. Could be segments of the same line, but we don't count that as a collision
	if (denominator == 0)
	{
		return std::nullopt;
	}
	else
	{
		if (pointTrajectory.IsVertical())
		{
			x = pointTrajectory.minX;
		}
		else if (line.IsVertical())
		{
			x = line.minX;
		}
		else
		{
			x = (pointTrajectory.constant * line.yCoefficient - line.constant * pointTrajectory.yCoefficient) / denominator;
		}
	}

	float y;
	// This will not work for a vertical line
	if (!line.TryYFromX(x, y))
	{
		// This must be a vertical line so this must work
		pointTrajectory.TryYFromX(x, y);
	}

	// Need to check both x and y to handle vertical/horizontal lines (because our answer will always be in x range for a vertical line, but may not be in y range, and vice-versa for horizontal)
	// For a vertical line at x = a, we know that our calculated collision point 'should' have x = a and any discrepancy is a floating point error; therefore we can bypass the potential
	// error by not checking it. We can apply the same logic for not checking y values for horizontal lines.
	if((!pointTrajectory.IsVertical() && !pointTrajectory.ExistsAtX(x))
		|| (!pointTrajectory.IsHorizontal() && !pointTrajectory.ExistsAtY(y))
		|| (!line.IsVertical() && !line.ExistsAtX(x))
		|| (!line.IsHorizontal() && !line.ExistsAtY(y)))
	{
		return std::nullopt;
	}

	Vector2F contactPoint = { x, y };

	auto normal = end - start;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(movement);
	bool stationarySide = dotProduct < 0;

	if ((stationarySide && internalityFilter == InternalityFilter::Internal) || (!stationarySide && internalityFilter == InternalityFilter::External))
	{
		return std::nullopt;
	}

	normal = normal * dotProduct;
	normal.Normalise();

	return Contact(-normal, contactPoint, stationarySide, true, Vector2F::DistanceBetween(contactPoint, other));
}

void Line::Translate(Vector2F amount)
{
	start += amount;
	end += amount;
}

void Line::SetCentre(Vector2F position)
{
	auto difference = end - start;
	start = position - difference / 2;
	end = position + difference / 2;
}

AxisAlignedRectF Line::GetAxisAlignedBoundingBox() const
{
	auto minX = std::min(start.x, end.x);
	auto minY = std::min(start.y, end.y);
	return AxisAlignedRectF(minX, minY, std::max(start.x, end.x) - minX, std::max(start.y, end.y) - minY);
}

std::optional<Contact> Line::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}
