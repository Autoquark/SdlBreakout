#include "stdafx.h"
#include <optional>
#include <algorithm>
#include <vector>
#include <limits>
#include "Collision.h"
#include "Line.h"
#include "Rectangle.h"
#include "RectF.h"

bool Collision::RectangleRectangleOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	int right1, right2;
	int bottom1, bottom2;

	//Calculate the sides of rect A
	right1 = rect1.x + rect1.w;
	bottom1 = rect1.y + rect1.h;

	//Calculate the sides of rect B
	right2 = rect2.x + rect2.w;
	bottom2 = rect2.y + rect2.h;

	if (bottom1 <= rect2.y)
	{
		return false;
	}

	if (rect1.y >= bottom2)
	{
		return false;
	}

	if (right1 <= rect2.x)
	{
		return false;
	}

	if (rect1.x >= right2)
	{
		return false;
	}

	return true;
}

//Returns the first contact between a point and a line, if any, when the point travels along the given line
std::optional<Contact> Collision::PointLineCast(const Vector2& pointStartPosition, const Vector2& pointEndPosition, const Vector2& linePoint1, const Vector2& linePoint2)
{
	auto line1 = GeneralFormLine(pointStartPosition, pointEndPosition);
	auto line2 = GeneralFormLine(linePoint1, linePoint2);

	auto denominator = line2.xCoefficient * line1.yCoefficient - line1.xCoefficient * line2.yCoefficient;
	
	float x;
	// Special case: two segments of the same line
	if (denominator == 0)
	{
		auto largestMin = max(line1.maxX, line2.maxX);
		auto smallestMax = min(line1.minX, line2.minX);

		// The segments do not overlap
		if (largestMin > smallestMax)
		{
			return std::nullopt;
		}

		// The overlapping range is from largestMin to smallestMax, use the smallest X value in the range
		x = largestMin;
	}
	else
	{
		x = (line1.constant * line2.yCoefficient - line2.constant * line1.yCoefficient)
			/ denominator;
	}

	auto y = (-line1.xCoefficient * x - line1.constant) / line1.yCoefficient;

	// Need to check both x and y to handle vertical lines
	if (x < line1.minX || x < line2.minX || x > line1.maxX || x > line2.maxX || y < line1.minY || y < line2.minY || y > line1.maxY || y > line2.maxY)
	{
		return std::nullopt;
	}

	Vector2 point = { x, y };

	auto normal = linePoint2 - linePoint1;
	normal.Rotate(90);
	normal = normal * normal.DotProduct(pointEndPosition - pointStartPosition);
	normal.Normalise();

	return Contact(-normal, point, point);
}

std::optional<Contact> Collision::PointRectangleCast(const Vector2 & pointStartPosition, const Vector2 & pointEndPosition, const RectF & rect)
{
	// Check if line starts and ends inside rectangle
	if (rect.Contains(pointStartPosition) && rect.Contains(pointEndPosition))
	{
		return Contact(Vector2(), pointStartPosition, pointStartPosition);
	}

	std::vector<std::optional<Contact>> contacts = {
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopLeft(), rect.TopRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.TopRight(), rect.BottomRight()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomRight(), rect.BottomLeft()),
		PointLineCast(pointStartPosition, pointEndPosition, rect.BottomLeft(), rect.TopLeft()),
	};

	//std::remove_if(contacts.begin(), contacts.end(), [](std::optional<Contact> c) { return c.has_value(); });

	// Find the first contact in the point's path
	auto bestDistance = (std::numeric_limits<float>::max)();
	auto bestContact = std::optional<Contact>();
	for (auto contact : contacts)
	{
		if (!contact.has_value())
		{
			continue;
		}

		auto distance = contact.value().centroid.DistanceTo(pointStartPosition);
		if (!bestContact.has_value() || distance < bestDistance)
		{
			bestDistance = distance;
			bestContact = contact.value();
		}
	}

	return bestContact;
}

std::optional<Contact> Collision::RectangleRectangleCast(const RectF & movingRect, const SDL_Rect & stationaryRect, const Vector2& movement)
{
	return std::nullopt;
}
