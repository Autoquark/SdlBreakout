#include "stdafx.h"
#include "CircleF.h"

std::optional<Contact> CircleF::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}

std::optional<Contact> CircleF::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    return std::optional<Contact>();
}
