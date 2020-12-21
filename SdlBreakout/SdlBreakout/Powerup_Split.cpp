#include "stdafx.h"
#include "Powerup_Split.h"

#include "Game.h"
#include "Ball.h"

void Powerup_Split::Activate()
{
	for (auto& ball : Game::GetLevel()->GetBalls())
	{
		auto newBall = Game::GetLevel()->AddGameObject(std::make_unique<Ball>(*ball));
		newBall->collisionBounds->SetCentre(ball->collisionBounds->GetCentre());
		newBall->SetDirection(ball->GetDirection().Rotated(45));
		ball->SetDirection(ball->GetDirection().Rotated(-45));
	}
}
