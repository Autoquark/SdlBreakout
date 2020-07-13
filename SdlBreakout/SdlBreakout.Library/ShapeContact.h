#pragma once
#include "Collision.h"

struct ShapeContact : Collision
{
public:
	ShapeContact(const Vector2& normal, const Vector2& point, bool isInternal, const Vector2& centroid);
	ShapeContact(const Vector2& normal, const Vector2& point, bool isInternal);

	bool isInternal;
};
