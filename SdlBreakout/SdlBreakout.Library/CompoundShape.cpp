#include "stdafx.h"

#include <optional>
#include <assert.h>

#include "CompoundShape.h"
#include "Shape.h"
#include "CircleF.h"

std::optional<Contact> CompoundShape::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	std::optional<Contact> bestContact = std::nullopt;
	for (auto shape : shapes)
	{
		bestContact = ClosestContact(bestContact, shape->CastAgainst(other, movement, internalityFilter));
	}
	return bestContact;
}

std::optional<Contact> CompoundShape::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Internal collisions are not currently supported for CompoundShape
	assert(internalityFilter == InternalityFilter::External);

	std::optional<Contact> bestContact = std::nullopt;
	for (auto shape : shapes)
	{
		bestContact = ClosestContact(bestContact, shape->CastAgainstThis(other, movement, internalityFilter));
	}
	return bestContact;
}

std::optional<Contact> CompoundShape::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Internal collisions are not currently supported for CompoundShape
	assert(internalityFilter == InternalityFilter::External);

	std::optional<Contact> bestContact = std::nullopt;
	for (auto shape : shapes)
	{
		bestContact = ClosestContact(bestContact, shape->CastAgainstThis(other, movement, internalityFilter));
	}
	return bestContact;
}

std::optional<Contact> CompoundShape::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Internal collisions are not currently supported for CompoundShape
	assert(internalityFilter == InternalityFilter::External);

	std::optional<Contact> bestContact = std::nullopt;
	for (auto shape : shapes)
	{
		bestContact = ClosestContact(bestContact, shape->CastAgainstThis(other, movement, internalityFilter));
	}
	return bestContact;
}

std::optional<Contact> CompoundShape::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	// Internal collisions are not currently supported for CompoundShape
	assert(internalityFilter == InternalityFilter::External);

	std::optional<Contact> bestContact = std::nullopt;
	for (auto shape : shapes)
	{
		bestContact = ClosestContact(bestContact, shape->CastAgainstThis(other, movement, internalityFilter));
	}
	return bestContact;
}

void CompoundShape::Translate(Vector2F amount)
{
	for (auto shape : shapes)
	{
		shape->Translate(amount);
	}
}

void CompoundShape::SetCentre(Vector2F position)
{
	auto centre = GetAxisAlignedBoundingBox().Centre();
	this->Translate(position - centre);
}

AxisAlignedRectF CompoundShape::GetAxisAlignedBoundingBox() const
{
	auto bounds = shapes[0]->GetAxisAlignedBoundingBox();
	for (auto shape : shapes)
	{
		bounds.Union(shape->GetAxisAlignedBoundingBox());
	}
	return bounds;
}