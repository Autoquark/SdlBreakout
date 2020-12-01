#pragma once
#include "GameObject.h"

class AxisAlignedRectF;

class Paddle : public GameObject
{
public:
	static const float MAX_VIRTUAL_CURVE;

	Paddle();
	~Paddle();

	const AxisAlignedRectF* centreSegment;

	void Update(float timeElapsed) override;

private:
	float moveSpeed;
};

