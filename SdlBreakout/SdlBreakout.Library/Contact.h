#pragma once

#include "stdafx.h"
#include "Vector2.h"
#include <assert.h>

struct Contact
{
public:
	static const float MIN_SEPARATION_DISTANCE;

	Contact(const Vector2F& normal, const Vector2F& point, const bool stationarySide, const bool movingSide, const float distance, const Vector2F& centroid)
		: normal(normal), point(point), stationarySide(stationarySide), movingSide(movingSide), distance(distance), centroid(centroid)
	{
		// Check for inside on inside collisions which are impossible
		assert(stationarySide || movingSide);
	}

	Contact(const Vector2F& normal, const Vector2F& point, const bool stationarySide, const bool movingSide, const float distance)
		: Contact(normal, point, stationarySide, movingSide, distance, point)
	{
	}
	~Contact();

	// Normal of the collision; this is a vector pointing out from the surface of the stationary object at the point of contact
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

	
	[[nodiscard]]
	/// <summary>
	/// Given a Contact resulting from a collision between an object A making a given direction M and stationary object B, returns the collision with A that would result from object B making direction -M.
	/// </summary>
	/// <param name="direction">Vector of any length representing the direction of movement A, the moving object in the original collision</param>
	/// <param name="centroid">The initial centre position of object B, the stationary object in the original collision</param>
	/// <returns></returns>
	Contact Invert(const Vector2F& direction, const Vector2F& centre) const
	{
		return Contact(-normal,
			point - direction.WithMagnitude(distance),
			movingSide,
			stationarySide,
			distance,
			centre - direction.WithMagnitude(distance));
	}
};

