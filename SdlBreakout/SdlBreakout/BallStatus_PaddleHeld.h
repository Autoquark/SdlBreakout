#pragma once
#include "stdafx.h"

#include "BallStatusEffect.h"
#include "Game.h"
#include "Ball.h"

// Status for when the ball is held by a sticky paddle
class BallStatus_PaddleHeld : public BallStatusEffect
{
public:
	BallStatus_PaddleHeld()
	{
		speedMultiplier = 0;
	}

	// Inherited via BallStatusEffect
	virtual void ApplyToBall(Ball* ball) const override;

	virtual void OnApplied(Ball* ball) override;

	virtual void Update(float timeElapsed) override;

private:
	Vector2F relativePosition = Vector2F();
};

