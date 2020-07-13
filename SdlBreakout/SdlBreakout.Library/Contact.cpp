#include "stdafx.h"
#include "Contact.h"


Contact::Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance, const Vector2& centroid) : normal(normal), point(point), side(side), distance(distance), centroid(centroid)
{
}

Contact::Contact(const Vector2& normal, const Vector2& point, const bool side, const float distance) : Contact(normal, point, side, distance, point)
{
}

Contact::~Contact()
{
}
