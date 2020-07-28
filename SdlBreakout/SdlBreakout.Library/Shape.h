#pragma once

#include <optional>
#include <vector>
#include "Contact.h"
#include "OptionalUtilities.h"
//#include "Collision.h"

class CircleF;
class AxisAlignedRectF;
class Point;
class Line;

class Shape
{
public:
	enum class InternalityFilter
	{
		Both,
		Internal,
		External
	};

	// Return the first contact with the given shape, if any, when this shape is moved along the given vector
	virtual std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	//virtual std::optional<PolygonContact> CastAgainstRect(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given axis aligned rectangle's first contact with this shape, if any, when the other shape is moved along the given vector
	virtual std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given circle's first contact with this shape, if any, when the circle is moved along the given vector
	virtual std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given lines' first contact with this shape, if any, when the line is moved along the given vector
	virtual std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given point's first contact with this shape, if any, when the point is moved along the given vector
	virtual std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	virtual AxisAlignedRectF GetAxisAlignedBoundingBox() const = 0;

	virtual void SetCentre(Vector2F position) = 0;
	void SetCentre(float x, float y)
	{
		SetCentre(Vector2F(x, y));
	}

	virtual void Translate(Vector2F amount) = 0;
	void Translate(float x, float y)
	{
		Translate(Vector2F(x, y));
	}

	template<class ElementType>
	static int FindClosestCollisionIndex(const std::vector<ElementType>& contacts, const InternalityFilter& internalityFilter = InternalityFilter::Both)
	{
		int bestIndex = -1;
		for (unsigned int i = 0; i < contacts.size(); i++)
		{
			auto nullable = contacts[i];
			if (!optionalUtilities::HasValue(nullable))
			{
				continue;
			}
			auto contact = *nullable;
			if ((contact.side && internalityFilter == InternalityFilter::Internal) || (!contact.side && internalityFilter == InternalityFilter::External))
			{
				continue;
			}

			if (bestIndex == -1 || (contact.distance < contacts[bestIndex]->distance))
			{
				bestIndex = i;
			}
		}

		return bestIndex;
	}

	template<class ElementType>
	static ElementType FindClosestCollision(const std::vector<ElementType>& contacts, const InternalityFilter& internalityFilter = InternalityFilter::Both)
	{
		auto index = FindClosestCollisionIndex(contacts, internalityFilter);
		return index == -1 ? std::nullopt : contacts[index];
	}
};