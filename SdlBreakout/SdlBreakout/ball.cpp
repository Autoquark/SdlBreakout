#include "stdafx.h"

#include <algorithm>
#include <iostream>

#include "ball.h"
#include "game.h"
#include "Vector2.h"
#include "Textures.h"
#include "CircleF.h"
#include "Debug.h"

Ball::Ball() : GameObject(new CircleF(0, 0, 8))
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
	auto effectiveSpeed = speed;

	// Only the single largest speed buff and single largest speed debuff apply
	auto iterator = std::max_element(statusEffects.begin(), statusEffects.end(), [](auto& x, auto& y) { return y->GetSpeedMultiplier() > x->GetSpeedMultiplier(); });
	if (iterator != statusEffects.end())
	{
		auto value = (*iterator)->GetSpeedMultiplier();
		if (value > 1)
		{
			effectiveSpeed *= value;
		}
	}

	iterator = std::min_element(statusEffects.begin(), statusEffects.end(), [](auto& x, auto& y) { return y->GetSpeedMultiplier() > x->GetSpeedMultiplier(); });
	if (iterator != statusEffects.end())
	{
		auto value = (*iterator)->GetSpeedMultiplier();
		if (value < 1)
		{
			effectiveSpeed *= value;
		}
	}

	Debug::PrintChanges("effectiveSpeed: ", std::to_string(effectiveSpeed));

	velocity.SetMagnitude(effectiveSpeed);
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
			auto block = game.GetBlocks()[index - 2];
			for (auto& status : statusEffects)
			{
				status->OnHitBlock(*block);
			}
			block->OnBallHit(*this);
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
			}
		}

		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds->SetCentre(collision.centroid);
		remainingVelocity.Reflect(normal);
		velocity.Reflect(normal);
	}

	for(auto& status : statusEffects)
	{
		status->Update(timeElapsed);
	}
	
	statusEffects.erase(std::remove_if(statusEffects.begin(), statusEffects.end(), [this](auto const& x) { return toRemove.find(x.get()) != toRemove.end(); }), statusEffects.end());
	toRemove.clear();

	GameObject::Update(timeElapsed);
}