#include "stdafx.h"

#include <optional>
#include "Shape.h"
#include "AxisAlignedRectF.h"
#include "Line.h"
#include "Point.h"
#include "CircleF.h"

Line AxisAlignedRectF::TopEdge() const
{
	return Line(TopLeft(), TopRight());
}

Line AxisAlignedRectF::RightEdge() const
{
	return Line(TopRight(), BottomRight());
}

Line AxisAlignedRectF::BottomEdge() const
{
	return Line(BottomRight(), BottomLeft());
}

Line AxisAlignedRectF::LeftEdge() const
{
	return Line(BottomLeft(), TopLeft());
}

std::optional<Contact> AxisAlignedRectF::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return other.CastAgainstThis(*this, movement, internalityFilter);
}

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter stationaryInternalityFilter) const
{
	auto movingCentre = Point(other.Centre());

	std::vector<std::optional<Contact>> contacts;

	if (stationaryInternalityFilter != InternalityFilter::Internal)
	{
		// Check for external collision by reducing the moving rectangle to a point and expanding this rectangle to compensate
		contacts.push_back(
			AxisAlignedRectF::FromCentre(Centre(), size.x + other.size.x, size.y + other.size.y)
			.CastAgainstThis(movingCentre,
				movement,
				InternalityFilter::External));
	}
	if (stationaryInternalityFilter != InternalityFilter::External)
	{
		// Check for internal collision by reducing the moving rectangle to a point and shrinking this rectangle to compensate
		contacts.push_back(
			AxisAlignedRectF::FromCentre(Centre(), size.x - other.size.x, size.y - other.size.y)
			.CastAgainstThis(movingCentre,
				movement,
				InternalityFilter::Internal));
	}

	auto nullableContact = FindClosestCollision(contacts, stationaryInternalityFilter);

	if (!nullableContact.has_value())
	{
		return std::nullopt;
	}

	auto contact = nullableContact.value();
	Vector2F point;
	auto distance = Vector2F::DistanceBetween(contact.point, movingCentre);
	if (contact.stationarySide)
	{
		// For an external collision, cast a point from the position of the moving rect at collision to the centre of the stationary rect
		auto temp = CastAgainstThis(Point(contact.centroid), Centre() - contact.centroid, InternalityFilter::External);
		point = temp.value().point;
	}
	else
	{
		// For an internal collision return the centre of the side of the moving rect which collided
		auto reducedRect = AxisAlignedRectF::FromCentre(Centre(), size.x - other.size.x, size.y - other.size.y);
		auto angle = (reducedRect.TopLeft() - reducedRect.Centre()).SignedAngleToDegrees(movement);

		// Bottom
		if (angle < -90)
		{
			point = Vector2F(movingCentre.position.x, other.Bottom());
		}
		// Left
		else if (angle < 0)
		{
			point = Vector2F(other.Left(), movingCentre.position.y);
		}
		// Top
		else if (angle < 90)
		{
			point = Vector2F(movingCentre.position.x, other.Top());
		}
		// Right
		else
		{
			point = Vector2F(other.Right(), movingCentre.position.y);
		}
		point += distance * movement.Normalised();
	}

	return Contact(contact.normal,
		point,
		contact.stationarySide,
		true, // TODO: Case when moving rectangle envelops stationary one
		distance,
		contact.point);
}

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	std::vector<std::optional<Contact>> contacts;
	auto point = Point(other.centre);
	if (internalityFilter != InternalityFilter::Internal)
	{
		// Check for external collisions by reducing the circle to a point and effectively checking against a larger rectangle with rounded corners.
		// First, move each of the sides of the rectangle out and check against them. The sides are not lengthened, so we can't delegate to the moving point vs stationary rectangle method.
		// For the corners, check the point against stationary circles at the original rectangle's corners with radius equal to the moving circle's
		auto edge = TopEdge();
		edge.Translate(0.0f, -other.radius);
		contacts.push_back(edge.CastAgainstThis(point, movement, InternalityFilter::External));

		edge = RightEdge();
		edge.Translate(other.radius, 0.0f);
		contacts.push_back(edge.CastAgainstThis(point, movement, InternalityFilter::External));

		edge = BottomEdge();
		edge.Translate(0.0f, other.radius);
		contacts.push_back(edge.CastAgainstThis(point, movement, InternalityFilter::External));

		edge = LeftEdge();
		edge.Translate(-other.radius, 0.0f);
		contacts.push_back(edge.CastAgainstThis(point, movement, InternalityFilter::External));

		// TODO: Check angle, we're only interested in 1/4 of each circle
		auto contact = CircleF(TopLeft(), other.radius).CastAgainstThis(point, movement, InternalityFilter::External);
		if (contact.has_value())
		{
			auto angle = (contact.value().point - other.centre).SignedAngleFromUp();
			if (angle >= -90 && angle <= 0)
			{
				contacts.push_back(contact);
			}
		}
		contact = CircleF(TopRight(), other.radius).CastAgainstThis(point, movement, InternalityFilter::External);
		if (contact.has_value())
		{
			auto angle = (contact.value().point - other.centre).SignedAngleFromUp();
			if (angle >= 0 && angle <= 90)
			{
				contacts.push_back(contact);
			}
		}
		contact = CircleF(BottomRight(), other.radius).CastAgainstThis(point, movement, InternalityFilter::External);
		if (contact.has_value())
		{
			auto angle = (contact.value().point - other.centre).SignedAngleFromUp();
			if (angle >= 90 && angle <= 180)
			{
				contacts.push_back(contact);
			}
		}
		contact = CircleF(BottomLeft(), other.radius).CastAgainstThis(point, movement, InternalityFilter::External);
		if (contact.has_value())
		{
			auto angle = (contact.value().point - other.centre).SignedAngleFromUp();
			if (angle == 180 || (angle >= -180 && angle <= -90))
			{
				contacts.push_back(contact);
			}
		}
	}

	if (internalityFilter != InternalityFilter::External)
	{
		// Check for internal collisions by reducing the circle to a point and shrinking the rectangle.
		contacts.push_back(AxisAlignedRectF::FromCentre(Centre(), size.x - other.radius, size.y - other.radius).CastAgainstThis(point, movement, InternalityFilter::Internal));
	}

	auto nullable = FindClosestCollision(contacts, internalityFilter);

	if (!nullable.has_value())
	{
		return std::nullopt;
	}

	auto contact = nullable.value();

	return Contact(contact.normal,
		contact.point + movement.WithLength(other.radius),
		contact.stationarySide,
		true, // TODO: What if the circle encloses the rectangle?
		contact.distance - other.radius,
		contact.point);
}

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Check if line starts and ends inside rectangle
	if (Contains(other) && Contains(other + movement))
	{
		return std::nullopt;
	}

	// Defining the vectors in a clockwise cycle ensures that the meaning of the Contact.side value remains consistent
	std::vector<std::optional<Contact>> contacts = {
		TopEdge().CastAgainstThis(other, movement, internalityFilter),
		RightEdge().CastAgainstThis(other, movement, internalityFilter),
		BottomEdge().CastAgainstThis(other, movement, internalityFilter),
		LeftEdge().CastAgainstThis(other, movement, internalityFilter),
	};

	return FindClosestCollision(contacts, internalityFilter);
}

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

void AxisAlignedRectF::Translate(Vector2F amount)
{
	position += amount;
}
