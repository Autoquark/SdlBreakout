#pragma once
#include "stdafx.h"

#include <memory>

#include "GameObject.h"
#include "Powerup.h"
#include "AxisAlignedRectF.h"

class PowerupDrop : public GameObject
{
public:
	explicit PowerupDrop(std::unique_ptr<Powerup> powerup) : GameObject(AxisAlignedRectF(0, 0, 32, 32)), powerup(std::move(powerup))
	{
		sprite = this->powerup->GetIcon();
	}
	virtual ~PowerupDrop() override = default;

	virtual void Update(float timeElapsed) override;

	void Collect();

private:
	static const float ACCELERATION;
	static const float MAX_SPEED;

	// Current velocity in the y axis in pixels per second
	float yVelocity = 0;
	std::unique_ptr<Powerup> powerup;
};

