#include "stdafx.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <limits>
#include "Collision.h"
#include "GeneralFormLine.h"
#include "Rectangle.h"
#include "AxisAlignedRectF.h"

//Returns the first nullableContact between a point and a line, if any, when the point travels along the given line
std::optional<PolygonContact> Collision::PointLineCast(const Vector2F& pointStartPosition, const Vector2F& pointEndPosition, const Vector2F& linePoint1, const Vector2F& linePoint2)
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

	return PolygonContact(-normal, point, dotProduct < 0, Vector2F::DistanceBetween(point, pointStartPosition), linePoint1, linePoint2);
}

std::optional<Contact> Collision::PointCircleCast(const Vector2F& pointStartPosition, const Vector2F& pointEndPosition, const CircleF& circle, InternalityFilter internalityFilter)
{
	auto movement = pointEndPosition - pointStartPosition;
	auto centreToStart = pointStartPosition - circle.centre;

	auto a = movement.DotProduct(movement);
	auto b = 2 * centreToStart.DotProduct(movement);
	auto c = centreToStart.DotProduct(centreToStart) - circle.radius * circle.radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return std::nullopt;
	}

	discriminant = std::sqrt(discriminant);
	float t1 = (-b - discriminant) / (2 * a);
	float t2 = (-b + discriminant) / (2 * a);

	if (t1 >= 0 && t1 <= 1 && internalityFilter != InternalityFilter::Internal)
	{
		// t1 is the intersection, and it's closer than t2
		// (since t1 uses -b - discriminant)
		// This must be an external hit
		auto point = Vector2F::LinearInterpolate(pointStartPosition, pointEndPosition, t1);
		return Contact((point - circle.centre).Normalised(), point, true, (point - pointStartPosition).Magnitude());
	}
	else if (t2 >= 0 && t2 <= 1 && internalityFilter != InternalityFilter::External)
	{
		// Internal hit
		auto point = Vector2F::LinearInterpolate(pointStartPosition, pointEndPosition, t2);
		return Contact((circle.centre - point).Normalised(), point, false, (point - pointStartPosition).Magnitude());
	}

	return std::nullopt;
}

std::optional<Contact> Collision::CircleLineCast(const CircleF& circle, const Vector2F& lineStart, const Vector2F& lineEnd, const Vector2F& movement)
{
	// Can currently only handle horizontal or vertical lines
	if (lineStart.x != lineEnd.x && lineStart.y != lineEnd.y)
	{
		throw std::exception();
	}

	auto circleEndCentre = circle.centre + movement;
	auto linePerpendicular = (lineEnd - lineStart).Rotated(90)
		.Normalised();
	auto rect = AxisAlignedRectF::FromCentre(Vector2F::LinearInterpolate(lineStart, lineEnd, 0.5f), Vector2F::DistanceBetween(lineStart, lineEnd), circle.radius * 2);
	if (lineStart.x == lineEnd.x)
	{
		rect.Rotate90();
	}
	std::vector<std::optional<Contact>> contacts = {
		PointRectangleCast(circle.centre, circleEndCentre, rect),
		PointCircleCast(circle.centre, circleEndCentre, CircleF(lineStart, circle.radius)),
		PointCircleCast(circle.centre, circleEndCentre, CircleF(lineEnd, circle.radius))
	};

	auto nullable = FindClosestCollision(contacts);
	if (!nullable.has_value())
	{
		return std::nullopt;
	}
	auto contact = *nullable;

	auto normal = lineEnd - lineStart;
	normal.Rotate(90);
	auto dotProduct = normal.DotProduct(circleEndCentre - circle.centre);

	return Contact(contact.normal, contact.point + circle.radius * movement.Normalised(), dotProduct < 0, contact.distance, contact.point);
}

std::optional<PolygonContact> Collision::PointRectangleCast(const Vector2F & pointStartPosition, const Vector2F & pointEndPosition, const AxisAlignedRectF & rect, const InternalityFilter internalityFilter)
{
	// Check if line starts and ends inside rectangle
	if (rect.Contains(pointStartPosition) && rect.Contains(pointEndPosition))
	{
		return std::nullopt;
	}

	// Defining the vectors in a clockwise cycle ensures that the meaning of the Contact.side value remains consistent
	std::vector<std::optional<PolygonContact>> contacts = {
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopLeft(), rect.TopRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopRight(), rect.BottomRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomRight(), rect.BottomLeft()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomLeft(), rect.TopLeft()),
	};

	return FindClosestCollision(contacts, internalityFilter);
}

std::optional<PolygonContact> Collision::RectangleRectangleCast(const AxisAlignedRectF & movingRect, const AxisAlignedRectF & stationaryRect, const Vector2F& movement, const InternalityFilter internalityFilter)
{
	auto movingCentre = movingRect.Centre();

	std::vector<std::optional<PolygonContact>> contacts;

	if (internalityFilter != InternalityFilter::Internal)
	{
		contacts.push_back(
			PointRectangleCast(movingCentre,
				movingCentre + movement,
				AxisAlignedRectF::FromCentre(stationaryRect.Centre(), stationaryRect.size.x + movingRect.size.x, stationaryRect.size.y + movingRect.size.y),
				InternalityFilter::External));
	}
	if (internalityFilter != InternalityFilter::External)
	{
		contacts.push_back(
			PointRectangleCast(movingCentre,
				movingCentre + movement,
				AxisAlignedRectF::FromCentre(stationaryRect.Centre(), stationaryRect.size.x - movingRect.size.x, stationaryRect.size.y - movingRect.size.y),
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

	return PolygonContact(contact.normal,
		point,
		contact.side,
		distance,
		contact.collidedLineStart,
		contact.collidedLineEnd,
		contact.point);
	return std::nullopt;
}
