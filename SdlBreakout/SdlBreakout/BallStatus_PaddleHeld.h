#pragma once
#include "stdafx.h"

#include "BallStatusEffect.h"
#include "Game.h"

class Ball;

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
};

