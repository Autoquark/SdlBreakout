#include "Vector2.h"
#include <algorithm>

#pragma once
// Represents a line defined by an equation in the form ax + by + c = 0
class GeneralFormLine
{
public:
	GeneralFormLine(Vector2 point1, Vector2 point2)
	{
		if (point1.x > point2.x)
		{
			std::swap(point1, point2);
		}
		minX = point1.x;
		maxX = point2.x;
		minY = min(point1.y, point2.y);
		maxY = max(point1.y, point2.y);

		xCoefficient = point2.x - point1.x;
		yCoefficient = point1.y - point2.y;
		//constant = point1.x * point2.y - point2.x * point1.y;
		constant = (point1.x - point2.x) * point1.y + (point2.y - point1.y) * point1.x;
	}

	float minX;
	float maxX;
	float minY;
	float maxY;
	float xCoefficient;
	float yCoefficient;
	float constant;
};

