#include "stdafx.h"
#include "ShapeContact.h"

ShapeContact::ShapeContact(const Vector2 & normal, const Vector2 & point, bool isInternal, const Vector2 & centroid) : Collision(normal, point, centroid), isInternal(isInternal)
{
}

ShapeContact::ShapeContact(const Vector2 & normal, const Vector2 & point, bool isInternal) : ShapeContact(normal, point, isInternal, point)
{
}
