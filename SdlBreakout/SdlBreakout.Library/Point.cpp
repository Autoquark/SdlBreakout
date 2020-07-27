#include "stdafx.h"
#include "Point.h"

std::optional<Contact> Point::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return other.CastAgainstThis(*this, movement, internalityFilter);
}

std::optional<Contact> Point::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Point::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Point::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}

std::optional<Contact> Point::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
	return std::optional<Contact>();
}
