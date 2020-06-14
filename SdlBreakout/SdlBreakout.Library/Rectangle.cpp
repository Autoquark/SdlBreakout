#include "stdafx.h"
#include "Rectangle.h"

bool Contains(const SDL_Rect & rectangle, const Vector2 & point)
{
	return point.x >= rectangle.x
		&& point.x <= rectangle.x + rectangle.w
		&& point.y >= rectangle.y
		&& point.x <= rectangle.y + rectangle.h;
}
