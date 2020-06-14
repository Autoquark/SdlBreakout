#include "stdafx.h"
#include "Contact.h"



Contact::Contact(const Vector2& normal, const Vector2& point, const Vector2& centroid) : normal(normal), point(point), centroid(centroid)
{
}


Contact::~Contact()
{
}
