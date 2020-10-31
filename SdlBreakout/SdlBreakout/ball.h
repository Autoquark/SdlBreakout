#pragma once
#include "GameObject.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	void Update(float timeElapsed) override;
	
	void SetSpeed(float value)
	{
		speed = value;
		velocity.SetMagnitude(value);
	}

private:
	// Target speed of the ball. Stored separately to velocity as the magnitude of the velocity may vary slightly due to rounding error in physics calculations
	float speed;
	// Current velocity of the ball
	Vector2F velocity;
};

