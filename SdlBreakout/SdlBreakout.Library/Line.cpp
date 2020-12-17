#include "stdafx.h"
#include "Line.h"
#include "Point.h"
#include "GeneralFormLine.h"
#include "CircleF.h"
#include "AxisAlignedRectF.h"

std::optional<Contact> Line::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return other.CastAgainstThis(*this, movement, internalityFilter);
}

std::optional<Contact> Line::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return optionalUtilities::Apply<Contact, Contact>(other.CastAgainstThis(*this, -movement, internalityFilter), [&](auto x)
	{
		return x.Invert(-movement, other.Centre());
	});
}

std::optional<Contact> Line::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Reduce the circle to a point, and check against the line moved by the radius of the circle in each of the two perpendicular directions.
	// Also check against two circles placed at either end of the original line with radius equal to the radius of the original circle.
	auto circleEndCentre = other.centre + movement;

	auto offset = (end - start).Rotated(90).WithMagnitude(other.radius);
	auto line1 = Line(start + offset, end + offset);
	auto line2 = Line(start - offset, end - offset);

	auto point = Point(other.centre);

	std::optional<Contact> bestContact = line1.CastAgainstThis(point, movement);
	bestContact = ClosestContact(bestContact, line2.CastAgainstThis(point, movement));
	bestContact = ClosestContact(bestContact, CircleF(start, other.radius).CastAgainstThis(point, movement));
	bestContact = ClosestContact(bestContact, CircleF(end, other.radius).CastAgainstThis(point, movement));

	if (!bestContact.has_value())
	{
		return std::nullopt;
	}
	auto& contact = *bestContact;

	auto normal = end - start;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(circleEndCentre - other.centre);

	return Contact(contact.normal,
		// To find the actual contact point, take the actual centroid and move towards the surface that we hit (i.e. in the direction of -normal) by the radius of the circle
		contact.point - contact.normal.WithMagnitude(other.radius),
		dotProduct < 0,
		true /*TODO: Case where circle envelops line*/,
		contact.distance, contact.point);
}

std::optional<Contact> Line::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	auto pointTrajectory = GeneralFormLine(other, other + movement);
	auto line = GeneralFormLine(start, end);

	// For a zero movement vector, return false. Avoids issues with zero vectors
	if (movement == Vector2F())
	{
		return std::nullopt;
	}

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

std::optional<Contact> Line::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// When two lines collide, at least one endpoint of one line will be at the collision point.
	auto bestContact = CastAgainstThis(Point(other.start), movement, internalityFilter);

	bestContact = ClosestContact(bestContact, CastAgainstThis(Point(other.end), movement, internalityFilter));

	bestContact = ClosestContact(bestContact, optionalUtilities::Apply<Contact, Contact>(other.CastAgainstThis(Point(start), -movement, internalityFilter), [&](auto x)
	{
		return x.Invert(-movement, GetCentre());
	}));

	bestContact = ClosestContact(bestContact, optionalUtilities::Apply<Contact, Contact>(other.CastAgainstThis(Point(end), -movement, internalityFilter), [&](auto x)
	{
		return x.Invert(-movement, GetCentre());
	}));

	if (bestContact.has_value())
	{
		bestContact->stationarySide = SidednessValue(other.GetCentre()) > 0;
		bestContact->movingSide = SidednessValue(other.GetCentre()) < 0;
		bestContact->centroid = other.GetCentre() + movement.WithMagnitude(bestContact->distance);
	}
	return bestContact;
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
