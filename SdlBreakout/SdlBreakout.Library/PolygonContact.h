#pragma once

#include "stdafx.h"
#include "Contact.h"
#include "Vector2.h"

// Represents a collision where the stationary object is a polygon or line
class PolygonContact : public Contact
{
public:
	PolygonContact(const Vector2F& normal,
		const Vector2F& point,
		const bool side,
		const float distance,
		const Vector2F collidedLineStart,
		const Vector2F collidedLineEnd,
		const Vector2F& centroid) : Contact(normal, point, side, distance, centroid), collidedLineStart(collidedLineStart), collidedLineEnd(collidedLineEnd)
	{
	}

	PolygonContact(const Vector2F& normal, const Vector2F& point, const bool side, const float distance, const Vector2F collidedLineStart, const Vector2F collidedLineEnd)
		: Contact(normal, point, side, distance), collidedLineStart(collidedLineStart), collidedLineEnd(collidedLineEnd)
	{
	}

	// The start point of the line which was collided with
	Vector2F collidedLineStart;
	// The end point of the line which was collided with
	Vector2F collidedLineEnd;

	friend bool operator==(const PolygonContact& lhs, const PolygonContact& rhs)
	{
		return ((Contact)lhs) == ((Contact)rhs)
			&& lhs.collidedLineStart == rhs.collidedLineStart
			&& lhs.collidedLineEnd == rhs.collidedLineEnd;
	}
};

