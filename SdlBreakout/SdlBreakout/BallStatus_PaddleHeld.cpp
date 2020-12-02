#include "stdafx.h"
#include "BallStatus_PaddleHeld.h"

#include "Ball.h"

void BallStatus_PaddleHeld::ApplyToBall(Ball* ball) const
{
	auto status = std::make_unique<BallStatus_PaddleHeld>(*this);
	ball->AddStatus(std::move(status));
	ball->SetDirection(Vector2F::Down());
}

void BallStatus_PaddleHeld::OnApplied(Ball* ball)
{
	BallStatusEffect::OnApplied(ball);
	relativePosition = ball->collisionBounds->GetAxisAlignedBoundingBox().Centre() - Game::GetLevel()->paddle->collisionBounds->GetAxisAlignedBoundingBox().Centre();
}

void BallStatus_PaddleHeld::Update(float timeElapsed)
{
	BallStatusEffect::Update(timeElapsed);
	ball->collisionBounds->SetCentre(Game::GetLevel()->paddle->collisionBounds->GetAxisAlignedBoundingBox().Centre() + relativePosition);
}
