#pragma once

#include "stdafx.h"
#include "Vector2.h"

class CircleF
{
public:
	CircleF() = default;
	CircleF(Vector2F centre, float radius) : centre(centre), radius(radius){}
	CircleF(float x, float y, float radius) : CircleF(Vector2F(x, y), radius){}

	Vector2F centre;
	float radius;
};

