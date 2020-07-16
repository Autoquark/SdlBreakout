#include "stdafx.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <limits>
#include "Collision.h"
#include "GeneralFormLine.h"
#include "Rectangle.h"
#include "RectF.h"

int Collision::FindClosestCollisionIndex(std::vector<std::optional<Contact>>& contacts, const InternalityFilter& internalityFilter)
{
	int bestIndex = -1;
	for (unsigned int i = 0; i < contacts.size(); i++)
	{
		auto nullable = contacts[i];
		if (!nullable.has_value())
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

std::optional<Contact> Collision::FindClosestCollision(std::vector<std::optional<Contact>>& contacts, const InternalityFilter& internalityFilter)
{
	auto index = FindClosestCollisionIndex(contacts, internalityFilter);
	return index == -1 ? std::nullopt : contacts[index];
}

//Returns the first nullableContact between a point and a line, if any, when the point travels along the given line
std::optional<Contact> Collision::PointLineCast(const Vector2F& pointStartPosition, const Vector2F& pointEndPosition, const Vector2F& linePoint1, const Vector2F& linePoint2)
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

	Vector2F point = { x, y };

	auto normal = linePoint2 - linePoint1;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(pointEndPosition - pointStartPosition);
	normal = normal * dotProduct;
	normal.Normalise();

	return Contact(-normal, point, dotProduct < 0, Vector2F::DistanceBetween(point, pointStartPosition), linePoint1, linePoint2);
}

std::optional<Contact> Collision::PointRectangleCast(const Vector2F & pointStartPosition, const Vector2F & pointEndPosition, const RectF & rect, const InternalityFilter internalityFilter)
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

	auto x = contacts.begin();

	return FindClosestCollision(contacts, internalityFilter);
}

std::optional<Contact> Collision::RectangleRectangleCast(const RectF & movingRect, const RectF & stationaryRect, const Vector2F& movement, const InternalityFilter internalityFilter)
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
	Vector2F point;
	auto distance = Vector2F::DistanceBetween(contact.point, movingCentre);
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
			point = contact.collidedLineEnd.y < stationaryCentre.y ? Vector2F(movingCentre.x, movingRect.Top()) : Vector2F(movingRect.Right(), movingCentre.y);
		}
		// Either bottom or left
		else
		{
			point = contact.collidedLineEnd.y > stationaryCentre.y ? Vector2F(movingCentre.x, movingRect.Bottom()) : Vector2F(movingRect.Left(), movingCentre.y);
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
