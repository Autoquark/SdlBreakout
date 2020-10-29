#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include "ball.h"
#include "game.h"
#include "Vector2.h"
#include "Textures.h"
#include "CircleF.h"


Ball::Ball() : GameObject(new CircleF(0, 0, 8))
//Ball::Ball() : GameObject(new AxisAlignedRectF(0, 0, 16, 16))
{
	auto textures = Textures::GetTextures();
	sprite = textures["ball"s];
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
		auto screenEdgeCollision = collisionBounds->CastAgainst(*game.bounds->collisionBounds, remainingVelocity, Shape::InternalityFilter::Internal);
		auto paddleCollision = collisionBounds->CastAgainst(*paddle->collisionBounds, remainingVelocity, Shape::InternalityFilter::External);

		std::vector<std::optional<Contact>> contacts = {
			screenEdgeCollision,
			paddleCollision
		};

		for (auto block : game.GetBlocks())
		{
			contacts.push_back(collisionBounds->CastAgainst(*block->collisionBounds, remainingVelocity, Shape::InternalityFilter::External));
		}

		auto index = Shape::FindClosestCollisionIndex(contacts);
		if(index == -1)
		{
			collisionBounds->Translate(remainingVelocity);
			break;
		}

		if (index > 1)
		{
			game.GetBlocks()[index - 2]->OnBallHit(*this);
		}

		if (index == 1)
		{
			std::cout << "Ball hit paddle\n";
		}

		auto collision = *contacts[index];
		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds->SetCentre(collision.centroid);
		remainingVelocity.Reflect(collision.normal);
		velocity.Reflect(collision.normal);
	}

	GameObject::Update(timeElapsed);
}