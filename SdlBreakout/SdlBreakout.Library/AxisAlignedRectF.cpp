#include "stdafx.h"

#include <optional>
#include "Shape.h"
#include "AxisAlignedRectF.h"
#include "Line.h"
#include "Point.h"

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

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	auto movingCentre = Point(other.Centre());

	std::vector<std::optional<Contact>> contacts;

	if (internalityFilter != InternalityFilter::Internal)
	{
		contacts.push_back(
			AxisAlignedRectF::FromCentre(Centre(), size.x + other.size.x, size.y + other.size.y)
			.CastAgainstThis(movingCentre,
				movement,
				InternalityFilter::External));
	}
	if (internalityFilter != InternalityFilter::External)
	{
		contacts.push_back(
			AxisAlignedRectF::FromCentre(Centre(), size.x - other.size.x, size.y - other.size.y)
			.CastAgainstThis(movingCentre,
				movement,
				InternalityFilter::Internal));
	}

	auto nullableContact = FindClosestCollision(contacts, internalityFilter);

	if (!nullableContact.has_value())
	{
		return std::nullopt;
	}

	auto contact = nullableContact.value();
	Vector2F point;
	auto distance = Vector2F::DistanceBetween(contact.point, movingCentre);
	if (contact.side)
	{
		// For an external collision, cast a point from the position of the moving rect at collision to the centre of the stationary rect
		point = CastAgainstThis(Point(contact.centroid), Centre() - contact.centroid, InternalityFilter::External).value().point;
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
		contact.side,
		distance,
		contact.point);
}

std::optional<Contact> AxisAlignedRectF::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
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
	return std::optional<Contact>();
}
