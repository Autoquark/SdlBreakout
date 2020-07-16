#include "stdafx.h"
#include "Contact.h"


Contact::Contact(const Vector2F& normal, const Vector2F& point, const bool side, const float distance, const Vector2F collidedLineStart, const Vector2F collidedLineEnd, const Vector2F& centroid)
	: normal(normal), point(point), side(side), distance(distance), collidedLineStart(collidedLineStart), collidedLineEnd(collidedLineEnd), centroid(centroid)
{
}

Contact::Contact(const Vector2F& normal, const Vector2F& point, const bool side, const float distance, const Vector2F collidedLineStart, const Vector2F collidedLineEnd)
	: Contact(normal, point, side, distance, collidedLineStart, collidedLineEnd, point)
{
}

Contact::~Contact()
{
}
