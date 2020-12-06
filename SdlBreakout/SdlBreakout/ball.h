#pragma once
#include "stdafx.h"

#include <set>
#include <algorithm>
#include <memory>
#include <iostream>

#include "GameObject.h"
#include "BallStatusEffect.h"
#include "BallStatus_PaddleHeld.h"

class BallStatus_PaddleHeld;

class Ball : public GameObject
{
public:
	const static float BASE_SPEED;
	const static float SPEED_BOOST_DECAY_FACTOR;

	Ball();
	~Ball() = default;

	void Update(float timeElapsed) override;
	
	float GetSpeed()
	{
		return baseSpeed;
	}
	void SetBaseSpeed(float value)
	{
		baseSpeed = value;
	}
	void SetDirection(Vector2F value)
	{
		direction = value.Normalised();
	}

	void ApplySpeedBoost(float amout)
	{
		speedBoost += amout;
	}

	void AddStatus(std::unique_ptr<BallStatusEffect> status)
	{
		if (!status)
		{
			throw new std::exception();
		}
		statusEffects.push_back(std::move(status));
		statusEffects[statusEffects.size() - 1]->OnApplied(this);
	}

	void RemoveStatus(BallStatusEffect* status)
	{
		toRemove.insert(status);
	}

private:
	std::vector<std::unique_ptr<BallStatusEffect>> statusEffects;
	std::set<BallStatusEffect*> toRemove;
	// Target speed of the ball. Stored separately to velocity as the magnitude of the velocity may vary slightly due to rounding error in physics calculations
	float baseSpeed = BASE_SPEED;
	float speedBoost = 0;
	int hitChain = 0;
	// Current direction of the ball
	Vector2F direction = Vector2F::Down();
	BallStatus_PaddleHeld* heldStatus = nullptr;

	float CalculateEffectiveSpeed();
};

