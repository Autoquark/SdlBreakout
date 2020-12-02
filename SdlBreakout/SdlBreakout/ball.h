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
	const float BASE_SPEED = 190;

	Ball();
	~Ball() = default;

	void Update(float timeElapsed) override;
	
	float GetSpeed()
	{
		return speed;
	}
	void SetBaseSpeed(float value)
	{
		speed = value;
	}
	void SetDirection(Vector2F value)
	{
		direction = value.Normalised();
	}

	void AddStatus(std::unique_ptr<BallStatusEffect> status)
	{
		if (!status)
		{
			throw new std::exception();
		}
		std::cout << "Status added: " << status << std::endl;
		statusEffects.push_back(std::move(status));
		statusEffects[statusEffects.size() - 1]->OnApplied(this);
	}

	void RemoveStatus(BallStatusEffect* status)
	{
		std::cout << "Status removed: " << status << std::endl;
		toRemove.insert(status);
	}

private:
	std::vector<std::unique_ptr<BallStatusEffect>> statusEffects;
	std::set<BallStatusEffect*> toRemove;
	// Target speed of the ball. Stored separately to velocity as the magnitude of the velocity may vary slightly due to rounding error in physics calculations
	float speed = BASE_SPEED;
	// Current direction of the ball
	Vector2F direction = Vector2F::Down();
	BallStatus_PaddleHeld* heldStatus = nullptr;

	float CalculateEffectiveSpeed();
};

