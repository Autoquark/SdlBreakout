#pragma once
#include "stdafx.h"

#include "Paddle.h"
#include "Textures.h"

class Ball;
class Block;

class BallStatusEffect
{
public:
	BallStatusEffect() {}
	virtual ~BallStatusEffect() = default;

	virtual void ApplyToBall(Ball* ball) const = 0;

	virtual void OnApplied(Ball* ball)
	{
		this->ball = ball;
	}

	virtual void Update(float timeElapsed) {}

	virtual void OnRemoved(){}

	virtual void OnHitBlock(Block& block) {}

	virtual void OnHitPaddle(Paddle& paddle) {}

	float GetSpeedMultiplier() const
	{
		return speedMultiplier;
	}

	virtual const Texture* GetIcon() const
	{
		return Textures::GetTexture("missingStatus.png");
	}

protected:
	Ball* ball = nullptr;
	float speedMultiplier = 1;

private:
};

