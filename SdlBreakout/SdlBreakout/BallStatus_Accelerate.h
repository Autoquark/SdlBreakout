#pragma once
#include "stdafx.h"

#include "Ball.h"
#include "BallStatusEffect.h"
#include "Game.h"

class BallStatus_Accelerate : public BallStatusEffect
{
public:
	BallStatus_Accelerate() : BallStatusEffect(), removeAt(-1)
	{
		speedMultiplier = SPEED_FACTOR;
	}

	void Update(float timeElapsed) override
	{
		if (Game::GetInstance().GetTime() >= removeAt)
		{
			ball->RemoveStatus(this);
		}
	}

	void ApplyToBall(Ball* ball) const override
	{
		auto status = std::make_unique<BallStatus_Accelerate>(*this);
		status->removeAt = Game::GetInstance().GetTime() + DURATION;
		ball->AddStatus(std::move(status));
	}

	const Texture* GetIcon() const override
	{
		return Textures::GetTexture("accelerate");
	}

private:
	float removeAt;

	static const float SPEED_FACTOR;
	static const float DURATION;
};

