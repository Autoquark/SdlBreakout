#include "stdafx.h"

#include <numeric>

#include "Shape.h"
#include "AxisAlignedRectF.h"

Vector2F Shape::GetCentre() const
{
	return GetAxisAlignedBoundingBox().Centre();
}

std::optional<Contact> Shape::MoveToContact(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter)
{
	auto contact = CastAgainst(other, movement, internalityFilter);
	return MoveToContact(contact, movement);
}

Vector2F Shape::MoveToContact(const Contact& contact)
{
	auto movement = (contact.centroid - GetCentre());
	movement = movement.WithMagnitude(std::max(movement.Magnitude() - Contact::MIN_SEPARATION_DISTANCE, 0.0f));
	Translate(movement);
	return movement;
}

std::optional<Contact> Shape::MoveToContact(const std::optional<Contact>& contact, const Vector2F& movement)
{
	if (contact.has_value())
	{
		MoveToContact(*contact);
	}
	else
	{
		Translate(movement);
	}
	return contact;
}

std::optional<Contact> Shape::MoveToContact(const std::vector<const Shape*>& others, const Vector2F& movement, const InternalityFilter internalityFilter)
{
	return MoveToContact(
		std::accumulate(
			others.begin(),
			others.end(),
			std::optional<Contact>(),
			[&](std::optional<Contact> x, const Shape* y) { return ClosestContact(x, CastAgainst(*y, movement, internalityFilter)); }),
		movement);
}


