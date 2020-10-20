#pragma once
#include "Vector2.h"

struct Contact
{
public:
	/*static bool ApproxEquals(const Contact& lhs, const Contact& rhs, const float tolerance)
	{
		return 
	}*/

	// Normal of the collision
	Vector2F normal;
	// The point at which the objects touch
	Vector2F point;
	// The position of the centre of the shape when the collision occurred
	Vector2F centroid;
	// Value indicating which side of the stationary object was hit. A true value indicates that the moving object originated to the left of the vector defining the line hit.
	// For a collision against a closed shape, a true value always indicates an external collision and a false value an internal one
	bool stationarySide;
	// Value indicating which side of the moving object was hit. A true value indicates that the moving object originated to the left of the vector defining the line hit.
	// For a collision against a closed shape, a true value always indicates an external collision and a false value an internal one
	bool movingSide;
	// Distance the object moves before colliding
	float distance;
	
	friend bool operator==(const Contact& lhs, const Contact& rhs)
	{
		return lhs.normal == rhs.normal
			&& lhs.point == rhs.point
			&& lhs.centroid == rhs.centroid
			&& lhs.stationarySide == rhs.stationarySide
			&& lhs.distance == rhs.distance;
	}

	friend bool operator!=(const Contact& lhs, const Contact& rhs)
	{
		return !(lhs == rhs);
	}

	Contact(const Vector2F& normal, const Vector2F& point, const bool stationarySide, const bool movingSide, const float distance, const Vector2F& centroid)
		: normal(normal), point(point), stationarySide(stationarySide), distance(distance), centroid(centroid)
	{
	}

	Contact(const Vector2F& normal, const Vector2F& point, const bool stationarySide, const bool movingSide, const float distance)
		: Contact(normal, point, stationarySide, movingSide, distance, point)
	{
	}
	~Contact();
};

