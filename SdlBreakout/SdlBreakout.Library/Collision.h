#pragma once
#include <SDL.h>
#include <optional>
#include "Contact.h"
#include "Vector2.h"

class Collision
{
public:
	Collision() = delete;

	static bool RectangleRectangleOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

	// Finds the point of intersection between two lines. If the lines overlap, finds the point of intersection with the smallest X value
	static std::optional<Contact> PointLineCast(const Vector2& pointStartPosition, const Vector2& pointEndPosition, const Vector2& line2Point1, const Vector2& line2Point2);

	//static std::optional<Contact> 

	// Finds the first point of intersection between a line and a rectangle
	static std::optional<Contact> PointRectangleCast(const Vector2& lineStart, const Vector2& lineEnd, const SDL_Rect& rect);

	// Finds the first point of contact between a moving and stationary rectangle
	static std::optional<Contact> RectangleRectangleCast(const SDL_Rect& movingRect, const SDL_Rect& stationaryRect, const Vector2& movement);
};
