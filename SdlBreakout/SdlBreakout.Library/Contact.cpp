#include "stdafx.h"
#include "Contact.h"


Contact::Contact(const Vector2& normal, const Vector2& point, bool side, const Vector2& centroid) : normal(normal), point(point), side(side), centroid(centroid)
{
}

Contact::Contact(const Vector2& normal, const Vector2& point, bool side) : Contact(normal, point, side, point)
{
}

Contact::~Contact()
{
}
