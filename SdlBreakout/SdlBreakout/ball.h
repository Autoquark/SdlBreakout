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
	
	Vector2F GetDirection()
	{
		return direction;
	}
	void SetDirection(Vector2F value)
	{
		direction = value.Normalised();
	}

	void ApplySpeedBoost(float amount)
	{
		speedBoost += amount;
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
	const static float TIME_UNTIL_MAX_SPEED;
	const static float MAX_SPEED_MULTIPLIER;

	std::vector<std::unique_ptr<BallStatusEffect>> statusEffects;
	std::set<BallStatusEffect*> toRemove;
	float createdTime = -1;
	float speedBoost = 0;
	int hitChain = 0;
	// Current direction of the ball
	Vector2F direction = Vector2F::Down();
	BallStatus_PaddleHeld* heldStatus = nullptr;

	float CalculateEffectiveSpeed();
};

