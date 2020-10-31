#pragma once
#include "GameObject.h"
#include "AxisAlignedRectF.h"

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

