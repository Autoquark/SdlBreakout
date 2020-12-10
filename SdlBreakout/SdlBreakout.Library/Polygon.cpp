#include "stdafx.h"
#include "Polygon.h"

#include "AxisAlignedRectF.h"

std::unique_ptr<Shape> Polygon::Clone() const
{
    return std::make_unique<Polygon>(*this);
}

std::optional<Contact> Polygon::CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    std::optional<Contact> bestContact = std::nullopt;
    for (auto& side : sides)
    {
        bestContact = ClosestContact(bestContact, side.CastAgainst(other, movement, internalityFilter));
    }
    //TODO: Centroid
    return bestContact;
}

std::optional<Contact> Polygon::CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    std::optional<Contact> bestContact = std::nullopt;
    for (auto& side : sides)
    {
        bestContact = ClosestContact(bestContact, side.CastAgainstThis(other, movement, internalityFilter));
    }
    return bestContact;
}

std::optional<Contact> Polygon::CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    std::optional<Contact> bestContact = std::nullopt;
    for (auto& side : sides)
    {
        bestContact = ClosestContact(bestContact, side.CastAgainstThis(other, movement, internalityFilter));
    }
    return bestContact;
}

std::optional<Contact> Polygon::CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    std::optional<Contact> bestContact = std::nullopt;
    for (auto& side : sides)
    {
        bestContact = ClosestContact(bestContact, side.CastAgainstThis(other, movement, internalityFilter));
    }
    return bestContact;
}

std::optional<Contact> Polygon::CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter) const
{
    std::optional<Contact> bestContact = std::nullopt;
    for (auto& side : sides)
    {
        bestContact = ClosestContact(bestContact, side.CastAgainstThis(other, movement, internalityFilter));
    }
    return bestContact;
}

AxisAlignedRectF Polygon::GetAxisAlignedBoundingBox() const
{
    auto vertices = GetVertices();
    auto xValues = std::minmax_element(vertices.cbegin(), vertices.cend(), [](auto first, auto second) { return first.x < second.x; });
    auto yValues = std::minmax_element(vertices.cbegin(), vertices.cend(), [](auto first, auto second) { return first.y < second.y; });
    return AxisAlignedRectF(xValues.first->x, yValues.first->y, xValues.second->x - xValues.first->x, yValues.second->y - yValues.first->y);
}

void Polygon::SetCentre(Vector2F position)
{
    auto centre = GetAxisAlignedBoundingBox().Centre();
    Translate(position - centre);
}

void Polygon::Translate(Vector2F amount)
{
    for (auto& line : sides)
    {
        line.start += amount;
        line.end += amount;
    }
}
