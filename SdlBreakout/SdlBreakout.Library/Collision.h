#pragma once
#include <SDL.h>
#include <optional>
#include <vector>
#include "Contact.h"
#include "Vector2.h"
#include "AxisAlignedRectF.h"
#include "CircleF.h"
#include "PolygonContact.h"
#include "OptionalUtilities.h"

class Collision
{
public:
	enum class InternalityFilter
	{
		Both,
		Internal,
		External
	};

	Collision() = delete;

	
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
			if ((contact.stationarySide && internalityFilter == InternalityFilter::Internal) || (!contact.stationarySide && internalityFilter == InternalityFilter::External))
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

	// Finds the point of intersection between two lines. If the lines overlap, finds the point of intersection with the smallest X value
	static std::optional<PolygonContact> PointLineCast(const Vector2F& pointStartPosition, const Vector2F& pointEndPosition, const Vector2F& line2Point1, const Vector2F& line2Point2);

	static std::optional<Contact> PointCircleCast(const Vector2F& pointStartPosition, const Vector2F& pointEndPosition, const CircleF& circle, InternalityFilter internalityFilter = InternalityFilter::Both);

	static std::optional<Contact> CircleLineCast(const CircleF& circle, const Vector2F& lineStart, const Vector2F& lineEnd, const Vector2F& movement);

	// Finds the first point of intersection between a moving point and a rectangle
	static std::optional<PolygonContact> PointRectangleCast(const Vector2F& pointStart, const Vector2F& pointEnd, const AxisAlignedRectF & rect, const InternalityFilter internalityFilter = InternalityFilter::Both);


	// Finds the first point of contact between a moving and stationary rectangle
	static std::optional<PolygonContact> RectangleRectangleCast(const AxisAlignedRectF & movingRect, const AxisAlignedRectF & stationaryRect, const Vector2F & movement, const InternalityFilter internalityFilter = InternalityFilter::Both);
};
