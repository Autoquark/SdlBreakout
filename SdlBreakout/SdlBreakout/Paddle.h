#pragma once
#include "GameObject.h"
#include "Powerup.h"

class AxisAlignedRectF;

class Paddle : public GameObject
{
public:
	static const float MAX_VIRTUAL_CURVE;
	static const float BUMP_UP_TIME;
	static const float BUMP_DOWN_TIME;
	//static const float BUMP_COOLDOWN;
	static const float BUMP_DISTANCE;

	Paddle();

	Vector2F GetSpeedLastUpdate() const
	{
		return speedLastUpdate;
	}

	void SetPowerup(std::unique_ptr<Powerup> powerup)
	{
		this->powerup = std::move(powerup);
	}
	void Update(float timeElapsed) override;

	bool isSticky = false;

private:
	float moveSpeed;
	float lastBumpTime = -999;
	float bumpStartY = -1;
	Vector2F speedLastUpdate = Vector2F();
	std::unique_ptr<Powerup> powerup;
};

