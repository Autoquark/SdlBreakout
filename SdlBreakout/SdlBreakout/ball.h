#pragma once
#include "stdafx.h"

#include <set>
#include <algorithm>
#include <memory>
#include <iostream>

#include "GameObject.h"
#include "BallStatusEffect.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	void Update(float timeElapsed) override;
	
	float GetSpeed()
	{
		return speed;
	}
	void SetBaseSpeed(float value)
	{
		speed = value;
		velocity.SetMagnitude(value);
	}
	void SetDirection(Vector2F value)
	{
		velocity.SetDirection(value);
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
	float speed;
	// Current velocity of the ball
	Vector2F velocity;
};

