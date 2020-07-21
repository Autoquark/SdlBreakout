#include "stdafx.h"
#include <algorithm>
#include "ball.h"
#include "game.h"
#include "Collision.h"
#include "Vector2.h"


Ball::Ball() : GameObject(AxisAlignedRectF{ 0, 0, 16, 16 })
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
		auto screenEdgeCollision = Collision::RectangleRectangleCast(GetCollisionBounds(), game.screenRect, remainingVelocity, Collision::InternalityFilter::Internal);
		auto paddleCollision = Collision::RectangleRectangleCast(GetCollisionBounds(), paddle->GetCollisionBounds(), remainingVelocity, Collision::InternalityFilter::External);

		std::vector<std::optional<Contact>> contacts = {
			screenEdgeCollision,
			paddleCollision
		};

		for (auto block : game.GetBlocks())
		{
			contacts.push_back(Collision::RectangleRectangleCast(GetCollisionBounds(), block->GetCollisionBounds(), remainingVelocity, Collision::InternalityFilter::External));
		}

		auto index = Collision::FindClosestCollisionIndex(contacts);
		if(index == -1)
		{
			collisionBounds.SetPosition(collisionBounds.TopLeft() + remainingVelocity);
			break;
		}

		if (index > 1)
		{
			game.GetBlocks()[index - 2]->OnBallHit(*this);
		}

		auto collision = *contacts[index];
		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds.SetPosition(collision.centroid - (collisionBounds.size / 2.0f));
		remainingVelocity.Reflect(collision.normal);
		velocity.Reflect(collision.normal);
	}

	GameObject::Update(timeElapsed);
}