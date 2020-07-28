#include "stdafx.h"
#include <algorithm>
#include "ball.h"
#include "game.h"
#include "Collision.h"
#include "Vector2.h"


Ball::Ball() : GameObject(new AxisAlignedRectF{ 0, 0, 16, 16 })
{
	sprite = Game::GetInstance().gBallTexture;
	velocity.x = -30;
	velocity.y = -90;
}


Ball::~Ball()
{
}

void Ball::Update(float timeElapsed)
{
	auto& game = Game::GetInstance();
	auto remainingVelocity = velocity * timeElapsed;
	while (true)
	{
		auto paddle = game.paddle;
		auto screenEdgeCollision = collisionBounds->CastAgainst(game.screenRect, remainingVelocity, Shape::InternalityFilter::Internal);
		auto paddleCollision = collisionBounds->CastAgainst(*paddle->collisionBounds, remainingVelocity, Shape::InternalityFilter::External);

		std::vector<std::optional<Contact>> contacts = {
			screenEdgeCollision,
			paddleCollision
		};

		for (auto block : game.GetBlocks())
		{
			contacts.push_back(collisionBounds->CastAgainst(*block->collisionBounds, remainingVelocity, Shape::InternalityFilter::External));
		}

		auto index = Collision::FindClosestCollisionIndex(contacts);
		if(index == -1)
		{
			collisionBounds->Translate(remainingVelocity);
			break;
		}

		if (index > 1)
		{
			game.GetBlocks()[index - 2]->OnBallHit(*this);
		}

		auto collision = *contacts[index];
		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds->SetCentre(collision.centroid);
		remainingVelocity.Reflect(collision.normal);
		velocity.Reflect(collision.normal);
	}

	GameObject::Update(timeElapsed);
}