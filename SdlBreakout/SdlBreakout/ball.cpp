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
	sprite = Textures::GetTexture("ball"s);
	velocity.x = -60;
	velocity.y = -180;
	speed = velocity.Magnitude();
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

		auto collision = *contacts[index];

		// If we hit the paddle
		auto normal = collision.normal;
		if (index == 1)
		{
			auto centreSegment = game.paddle->centreSegment;
			auto contact = collisionBounds->CastAgainst(*centreSegment, remainingVelocity, Shape::InternalityFilter::External);
			if (contact.has_value() && contact.value().distance <= collision.distance)
			{
				auto curveProportion = (collision.point.x - centreSegment->Centre().x) * 2 / centreSegment->size.x;
				normal.Rotate(curveProportion * Paddle::MAX_VIRTUAL_CURVE);
				std::cout << "Curve proportion: " << curveProportion << " normal: " << normal.ToString();
			}
		}

		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds->SetCentre(collision.centroid);
		remainingVelocity.Reflect(normal);
		velocity.Reflect(normal);

		// Correct any variation in speed due to rounding error
		SetSpeed(speed);
	}

	GameObject::Update(timeElapsed);
}