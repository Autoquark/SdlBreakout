#include "stdafx.h"

#include <numeric>

#include "Shape.h"

std::optional<Contact> Shape::MoveToContact(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter)
{
	auto contact = CastAgainst(other, movement, internalityFilter);
	return MoveToContact(contact, movement);
}

std::optional<Contact> Shape::MoveToContact(const std::optional<Contact>& contact, const Vector2F& movement)
{
	auto actualMove = movement;
	if (contact.has_value())
	{
		actualMove = movement.WithMagnitude(std::max(contact->distance - Contact::MIN_SEPARATION_DISTANCE, 0.0f));
	}
	Translate(actualMove);
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


