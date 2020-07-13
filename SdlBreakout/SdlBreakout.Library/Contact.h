#pragma once
#include "Vector2.h"

struct Contact
{
public:
	// Normal of the collision
	Vector2 normal;
	// The point at which the objects touch
	Vector2 point;
	// The position of the collision shape when the collision occurred
	Vector2 centroid;
	// Value indicating which side of the stationary line was hit. For a collision against a shape, a true value always indicates an external collision and a false value an internal one
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

	Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance, const Vector2& centroid);
	Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance);
	~Contact();
};

