#include "stdafx.h"
#include "BallStatus_PaddleHeld.h"

#include "Ball.h"

void BallStatus_PaddleHeld::ApplyToBall(Ball* ball) const
{
	auto status = std::make_unique<BallStatus_PaddleHeld>(*this);
	ball->AddStatus(std::move(status));
	ball->SetDirection(Vector2F::Down());
}
