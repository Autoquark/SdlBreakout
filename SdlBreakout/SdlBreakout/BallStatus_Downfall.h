#pragma once
#include "stdafx.h"

#include <memory>

#include "Game.h"
#include "BallStatusEffect.h"

class BallStatus_Downfall : public BallStatusEffect
{
public:
	BallStatus_Downfall() : BallStatusEffect()
	{
		speedMultiplier = SPEED_FACTOR;
	}

	void ApplyToBall(Ball* ball) const override
	{
		ball->AddStatus(std::make_unique<BallStatus_Downfall>(*this));
		ball->SetDirection(Vector2F(0, 1));
	}

	void OnHitBlock(Block& block) override
	{
		ball->RemoveStatus(this);
	}

	void OnHitPaddle(Paddle& block) override
	{
		ball->RemoveStatus(this);
	}

	const Texture* GetIcon() const override
	{
		return Textures::GetTexture("downfall");
	}

private:
	static const float SPEED_FACTOR;
};

