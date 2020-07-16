#pragma once
#include "Vector2.h"

struct Contact
{
public:
	// Normal of the collision
	Vector2F normal;
	// The point at which the objects touch
	Vector2F point;
	// The position of the centre of the shape when the collision occurred
	Vector2F centroid;
	// The start point of the line which was collided with
	Vector2F collidedLineStart;
	// The end point of the line which was collided with
	Vector2F collidedLineEnd;
	// Value indicating which side of the stationary line was hit. A true value indicates that the moving object originated to the left of the vector defining the line hit.
	// For a collision against a shape, a true value always indicates an external collision and a false value an internal one
	bool side;
	// Distance the object moves before colliding
	float distance;
	
	friend bool operator==(const Contact& lhs, const Contact& rhs)
	{
		return lhs.normal == rhs.normal
			&& lhs.point == rhs.point
			&& lhs.centroid == rhs.centroid;
	}

	friend bool operator!=(const Contact& lhs, const Contact& rhs)
	{
		return !(lhs == rhs);
	}

	Contact(const Vector2F& normal, const Vector2F& point, const bool side, const float distance, const Vector2F collidedLineStart, const Vector2F collidedLineEnd, const Vector2F& centroid);
	Contact(const Vector2F& normal, const Vector2F& point, const bool side, const float distance, const Vector2F collidedLineStart, const Vector2F collidedLineEnd);
	~Contact();
};

