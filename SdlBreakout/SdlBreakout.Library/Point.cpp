#include "stdafx.h"
#include "Point.h"
#include "AxisAlignedRectF.h"

std::optional<Contact> Point::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return other.CastAgainstThis(*this, movement, internalityFilter);
}

std::optional<Contact> Point::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

std::optional<Contact> Point::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

std::optional<Contact> Point::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

std::optional<Contact> Point::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	throw new std::exception();
}

void Point::Translate(Vector2F amount)
{
	position += amount;
}

void Point::SetCentre(Vector2F position)
{
	this->position = position;
}

AxisAlignedRectF Point::GetAxisAlignedBoundingBox() const
{
	return AxisAlignedRectF(position, Vector2F(0, 0));
}
