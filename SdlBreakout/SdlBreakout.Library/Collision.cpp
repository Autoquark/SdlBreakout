#include "stdafx.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <limits>
#include "Collision.h"
#include "GeneralFormLine.h"
#include "Rectangle.h"
#include "RectF.h"

std::optional<Contact> Collision::FindClosestCollision(std::vector<std::optional<Contact>> contacts, const InternalityFilter internalityFilter)
{
	contacts.erase(std::remove_if(contacts.begin(), contacts.end(), [](auto x) { return !x.has_value(); }), contacts.end());

	std::vector<Contact> actualContacts;
	std::transform(contacts.begin(), contacts.end(), std::back_inserter(actualContacts), [](auto x) { return x.value(); });
	actualContacts.erase(
		std::remove_if(
			actualContacts.begin(),
			actualContacts.end(),
			[&](auto x) { return (x.side && internalityFilter == InternalityFilter::Internal) || (!x.side && internalityFilter == InternalityFilter::External); }),
		actualContacts.end());

	auto resultIterator = std::min_element(actualContacts.begin(), actualContacts.end(), [](auto first, auto second) { return first.distance < second.distance; });

	if (resultIterator != actualContacts.end())
	{
		return *resultIterator;
	}

	return std::nullopt;
}

//Returns the first nullableContact between a point and a line, if any, when the point travels along the given line
std::optional<Contact> Collision::PointLineCast(const Vector2& pointStartPosition, const Vector2& pointEndPosition, const Vector2& linePoint1, const Vector2& linePoint2)
{
	auto pointTrajectory = GeneralFormLine(pointStartPosition, pointEndPosition);
	auto line = GeneralFormLine(linePoint1, linePoint2);

	auto denominator = line.xCoefficient * pointTrajectory.yCoefficient - pointTrajectory.xCoefficient * line.yCoefficient;
	
	float x;
	// Special case: two segments of the same line
	if (denominator == 0)
	{
		return std::nullopt;
	}
	else
	{
		x = (pointTrajectory.constant * line.yCoefficient - line.constant * pointTrajectory.yCoefficient)
			/ denominator;
	}

	float y;
	if (!line.TryYFromX(x, y))
	{
		pointTrajectory.TryYFromX(x, y);
	}

	// Need to check both x and y to handle vertical lines
	if (x < pointTrajectory.minX || x < line.minX || x > pointTrajectory.maxX || x > line.maxX || y < pointTrajectory.minY || y < line.minY || y > pointTrajectory.maxY || y > line.maxY)
	{
		return std::nullopt;
	}

	Vector2 point = { x, y };

	auto normal = linePoint2 - linePoint1;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(pointEndPosition - pointStartPosition);
	normal = normal * dotProduct;
	normal.Normalise();

	auto temp = Vector2::DistanceBetween(point, pointStartPosition);
	return Contact(-normal, point, dotProduct < 0, Vector2::DistanceBetween(point, pointStartPosition), linePoint1, linePoint2);
}

std::optional<Contact> Collision::PointRectangleCast(const Vector2 & pointStartPosition, const Vector2 & pointEndPosition, const RectF & rect, const InternalityFilter internalityFilter)
{
	// Check if line starts and ends inside rectangle
	if (rect.Contains(pointStartPosition) && rect.Contains(pointEndPosition))
	{
		return std::nullopt;
	}

	// Defining the vectors in a clockwise cycle ensures that the meaning of the nullableContact side value remains consistent
	std::vector<std::optional<Contact>> contacts = {
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopLeft(), rect.TopRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopRight(), rect.BottomRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomRight(), rect.BottomLeft()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomLeft(), rect.TopLeft()),
	};

	return FindClosestCollision(contacts, internalityFilter);
}

std::optional<Contact> Collision::RectangleRectangleCast(const RectF & movingRect, const RectF & stationaryRect, const Vector2& movement, const InternalityFilter internalityFilter)
{
	//TODO: Moving rectangle inside stationary rectangle, need to shrink stationary rectangle
	// Maybe test against grown and shrunk stationary rectangle, using InternalityFilter and/or distance calculation to choose correct result?
	auto movingCentre = movingRect.Centre();

	std::vector<std::optional<Contact>> contacts;

	if (internalityFilter != InternalityFilter::Internal)
	{
		contacts.push_back(
			PointRectangleCast(movingCentre,
				movingCentre + movement,
				RectF::FromCentre(stationaryRect.Centre(), stationaryRect.width + movingRect.width, stationaryRect.height + movingRect.height),
				InternalityFilter::External));
	}
	if (internalityFilter != InternalityFilter::External)
	{
		contacts.push_back(
			PointRectangleCast(movingCentre,
				movingCentre + movement,
				RectF::FromCentre(stationaryRect.Centre(), stationaryRect.width - movingRect.width, stationaryRect.height - movingRect.height),
				InternalityFilter::Internal));
	}

	auto nullableContact = FindClosestCollision(contacts, internalityFilter);

	if (!nullableContact.has_value())
	{
		return std::nullopt;
	}

	auto contact = nullableContact.value();
	Vector2 point;
	auto distance = Vector2::DistanceBetween(contact.point, movingCentre);
	if (contact.side)
	{
		// For an external collision, cast a point from the position of the moving rect at collision to the centre of the stationary rect
		point = PointRectangleCast(contact.centroid, stationaryRect.Centre(), stationaryRect).value().point;
	}
	else
	{
		// For an internal collision return the centre of the side of the moving rect which collided
		auto stationaryCentre = stationaryRect.Centre();
		// Either top or right
		if (contact.collidedLineStart.y < stationaryCentre.y)
		{
			point = contact.collidedLineEnd.y < stationaryCentre.y ? Vector2(movingCentre.x, movingRect.Top()) : Vector2(movingRect.Right(), movingCentre.y);
		}
		// Either bottom or left
		else
		{
			point = contact.collidedLineEnd.y > stationaryCentre.y ? Vector2(movingCentre.x, movingRect.Bottom()) : Vector2(movingRect.Left(), movingCentre.y);
		}
		point += distance * movement.Normalised();
	}

	return Contact(contact.normal,
		point,
		contact.side,
		distance,
		contact.collidedLineStart,
		contact.collidedLineEnd,
		contact.point);
}
