#pragma once
#include "GameObject.h"
#include "Powerup.h"

class AxisAlignedRectF;

class Paddle : public GameObject
{
public:
	static const float MAX_VIRTUAL_CURVE;

	Paddle();

	//const AxisAlignedRectF* centreSegment;

	void SetPowerup(std::unique_ptr<Powerup> powerup)
	{
		this->powerup = std::move(powerup);
	}
	void Update(float timeElapsed) override;

	bool isSticky = false;

private:
	float moveSpeed;
	std::unique_ptr<Powerup> powerup;
};

