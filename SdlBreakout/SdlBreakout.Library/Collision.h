#pragma once
#include <SDL.h>
#include <optional>
#include "Contact.h"
#include "Vector2.h"
#include "RectF.h"

class Collision
{
public:
	enum InternalityFilter
	{
		Both,
		Internal,
		External
	};

	Collision() = delete;

	static bool RectangleRectangleOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

	// Finds the point of intersection between two lines. If the lines overlap, finds the point of intersection with the smallest X value
	static std::optional<Contact> PointLineCast(const Vector2& pointStartPosition, const Vector2& pointEndPosition, const Vector2& line2Point1, const Vector2& line2Point2);

	// Finds the first point of intersection between a moving point and a rectangle
	static std::optional<Contact> PointRectangleCast(const Vector2& pointStart, const Vector2& pointEnd, const RectF & rect, InternalityFilter internalityFilter = InternalityFilter::Both);

	// Finds the first point of contact between a moving and stationary rectangle
	static std::optional<Contact> RectangleRectangleCast(const RectF& movingRect, const SDL_Rect& stationaryRect, const Vector2& movement);
};
