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
	auto remainingVelocity = velocity * timeElapsed;
	while (true)
	{
		auto paddle = game->paddle;
		auto screenEdgeCollision = Collision::RectangleRectangleCast(RectF(position, size), game->screenRect, remainingVelocity, Collision::InternalityFilter::Internal);
		auto paddleCollision = Collision::RectangleRectangleCast(RectF(position, size), RectF(paddle->position, paddle->GetSize()), remainingVelocity, Collision::InternalityFilter::External);

		std::vector<std::optional<Contact>> contacts = {
			screenEdgeCollision,
			paddleCollision
		};

		auto index = Collision::FindClosestCollisionIndex(contacts);
		if(index == -1)
		{
			position += remainingVelocity;
			break;
		}
		auto collision = *contacts[index];
		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		position = collision.centroid - (sprite->GetSize() / 2.0f);
		remainingVelocity.Reflect(collision.normal);
		velocity.Reflect(collision.normal);
	}

	GameObject::Update(timeElapsed);
}