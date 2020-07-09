#include "stdafx.h"
#include <algorithm>
#include "ball.h"
#include "game.h"
#include "Collision.h"
#include "Vector2.h"


Ball::Ball() : GameObject(Vector2 { 16, 16 })
{
	sprite = Game::GetInstance()->gBallTexture;
	velocity.x = -20;
	velocity.y = -60;
}


Ball::~Ball()
{
}

void Ball::Update(float timeElapsed)
{
	auto game = Game::GetInstance();
	auto rect = game->screenRect;
	auto remainingVelocity = velocity * timeElapsed;
	while (true)
	{
		auto nextPosition = position + remainingVelocity;
		auto collision = Collision::PointRectangleCast(position, nextPosition, game->screenRect, Collision::InternalityFilter::Internal);
		if(!collision.has_value())
		{
			position = nextPosition;
			break;
		}
		remainingVelocity *= Vector2::DistanceBetween(position, nextPosition) / velocity.Magnitude();
		position = (*collision).centroid;
		remainingVelocity.Reflect((*collision).normal);
		velocity.Reflect((*collision).normal);
	}

	GameObject::Update(timeElapsed);
}