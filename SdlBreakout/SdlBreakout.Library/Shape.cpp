#include "stdafx.h"

#include "Shape.h"

std::optional<Contact> Shape::MoveToContact(const Shape& other, Vector2F& movement, const InternalityFilter internalityFilter)
{
	auto contact = CastAgainst(other, movement, internalityFilter);
	return MoveToContact(contact, movement);
}

std::optional<Contact> Shape::MoveToContact(const std::optional<Contact>& contact, Vector2F& movement)
{
	if (contact.has_value())
	{
		movement.SetMagnitude(std::max(contact->distance - Contact::MIN_SEPARATION_DISTANCE, 0.0f));
	}
	Translate(movement);
	return contact;
}

std::optional<Contact> Shape::MoveToContact(const std::vector<const Shape*>& others, Vector2F& movement, const InternalityFilter internalityFilter)
{
	std::optional<Contact> contact = std::nullopt;
	for (auto shape : others)
	{
		contact = ClosestContact(contact, CastAgainst(*shape, movement, internalityFilter));
	}
	return MoveToContact(contact, movement);
}


