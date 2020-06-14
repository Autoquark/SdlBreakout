#include "stdafx.h"
#include "Collision.h"
#include "Line.h"
#include <optional>
#include <algorithm>
#include "Rectangle.h"

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
std::optional<Contact> Collision::PointLineCast(const Vector2& pointStartPosition, const Vector2& pointEndPosition, const Vector2& line2Point1, const Vector2& line2Point2)
{
	auto line1 = GeneralFormLine(pointStartPosition, pointEndPosition);
	auto line2 = GeneralFormLine(line2Point1, line2Point2);

	auto denominator = line2.xCoefficient * line1.yCoefficient - line1.xCoefficient * line2.yCoefficient;
	
	float x;
	// Special case: two segments of the same line
	if (denominator == 0)
	{
		auto largestMin = max(line1.endX, line2.endX);
		auto smallestMax = min(line1.startX, line2.startX);

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

	//auto y = (line1.xCoefficient * line2.constant - line1.constant * line2.xCoefficient) / (line1.yCoefficient * line2.xCoefficient - line1.xCoefficient - line2.yCoefficient);
	//auto x = (-line1.yCoefficient * y - line1.constant) / line1.xCoefficient;

	if (x < line1.startX || x < line2.startX || x > line1.endX || x > line2.endX)
	{
		return std::nullopt;
	}

	Vector2 point = { x, y };
	//auto sideOfLine = (line2Point2.x - line2Point1.x) * (point.y - line2Point1.y) - (line2Point2.y - line2Point1.y) * (point.y - line2Point1.x); 

	auto normal = line2Point2 - line2Point1;
	normal.Rotate(90);

	return Contact(normal * normal.DotProduct(pointEndPosition - pointStartPosition), point, point);
}

std::optional<Contact> Collision::PointRectangleCast(const Vector2 & lineStart, const Vector2 & lineEnd, const SDL_Rect & rect)
{
	// Check if line starts and ends inside rectangle
	if (Contains(rect, lineStart) && Contains(rect, lineEnd))
	{
		return Contact(Vector2{}, lineStart, lineStart);
	}

	return std::nullopt;
}

std::optional<Contact> Collision::RectangleRectangleCast(const SDL_Rect & movingRect, const SDL_Rect & stationaryRect, const Vector2& movement)
{
	return std::nullopt;
}
