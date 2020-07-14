#include "stdafx.h"
#include "Contact.h"


Contact::Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance, const Vector2 collidedLineStart, const Vector2 collidedLineEnd, const Vector2& centroid)
	: normal(normal), point(point), side(side), distance(distance), collidedLineStart(collidedLineStart), collidedLineEnd(collidedLineEnd), centroid(centroid)
{
}

Contact::Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance, const Vector2 collidedLineStart, const Vector2 collidedLineEnd)
	: Contact(normal, point, side, distance, collidedLineStart, collidedLineEnd, point)
{
}

Contact::~Contact()
{
}
