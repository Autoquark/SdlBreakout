#include "stdafx.h"
#include "Ball.h"

#include <algorithm>
#include <iostream>

#include "Game.h"
#include "Vector2.h"
#include "Textures.h"
#include "CircleF.h"
#include "Debug.h"
#include "BallStatus_PaddleHeld.h"

Ball::Ball() : GameObject(new CircleF(0, 0, 8))
{
	sprite = Textures::GetTexture("ball"s);
}


void Ball::Update(float timeElapsed)
{
	auto level = Game::GetLevel();
	auto paddle = level->paddle;
	if (Game::GetInput().MouseButtonPressed(Input::MouseButton::Left) && heldStatus != nullptr)
	{
		RemoveStatus(heldStatus);
		heldStatus = nullptr;
		paddle->isSticky = false;
	}

	auto effectiveSpeed = CalculateEffectiveSpeed();

	auto remainingVelocity = direction * effectiveSpeed * timeElapsed;

	while (true)
	{
		auto screenEdgeCollision = collisionBounds->CastAgainst(Game::levelArea, remainingVelocity, Shape::InternalityFilter::Internal);
		auto paddleCollision = collisionBounds->CastAgainst(*paddle->collisionBounds, remainingVelocity, Shape::InternalityFilter::External);

		std::vector<std::optional<Contact>> contacts = {
			screenEdgeCollision,
			paddleCollision
		};

		auto& blocks = level->GetBlocks();
		std::transform(blocks.begin(), blocks.end(),
			std::back_inserter(contacts),
			[&](auto const block) { return collisionBounds->CastAgainst(*block->collisionBounds, remainingVelocity, Shape::InternalityFilter::External); });

		auto index = Shape::FindClosestCollisionIndex(contacts);
		if(index == -1)
		{
			collisionBounds->Translate(remainingVelocity);
			break;
		}

		auto& collision = *contacts[index];

		if (index > 1)
		{
			auto block = level->GetBlocks()[index - 2];
			for (auto& status : statusEffects)
			{
				status->OnHitBlock(*block);
			}
			block->OnBallHit(*this);
			if (!block->invulnerable)
			{
				Game::GetInstance().score += 10;
			}
		}

		auto normal = collision.normal;
		if (!Game::DISABLE_BALL_LOSS && index == 0 && std::abs(collision.point.y - Game::levelArea.Bottom()) < 1)
		{
			level->ScheduleDestroy(this);
			return;
		}
		// If we hit the paddle
		else if (index == 1)
		{
			if (paddle->isSticky)
			{
				collisionBounds->MoveToContact(collision, remainingVelocity);

				auto unique = std::make_unique<BallStatus_PaddleHeld>();
				heldStatus = unique.get();
				AddStatus(std::move(unique));
				break;
			}
			else
			{
				auto centreSegment = level->paddle->centreSegment;
				auto contact = collisionBounds->CastAgainst(*centreSegment, remainingVelocity, Shape::InternalityFilter::External);
				if (contact.has_value() && contact.value().distance <= collision.distance)
				{
					// We need to negate the value here because in SDL +y is down
					auto curveProportion = -(collision.point.x - centreSegment->Centre().x) * 2 / centreSegment->size.x;
					normal.Rotate(curveProportion * Paddle::MAX_VIRTUAL_CURVE);
				}
			}
		}

		remainingVelocity *= 1 - (collision.distance / remainingVelocity.Magnitude());
		collisionBounds->SetCentre(collision.centroid);
		remainingVelocity.Reflect(normal);
		direction.Reflect(normal);
	}

	for(auto& status : statusEffects)
	{
		status->Update(timeElapsed);
	}
	
	statusEffects.erase(std::remove_if(statusEffects.begin(), statusEffects.end(), [this](auto const& x) { return toRemove.find(x.get()) != toRemove.end(); }), statusEffects.end());
	toRemove.clear();

	speedBoost *= std::powf(SPEED_BOOST_DECAY_FACTOR, timeElapsed);

	GameObject::Update(timeElapsed);
}

float Ball::CalculateEffectiveSpeed()
{
	auto effectiveSpeed = baseSpeed;
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

	return effectiveSpeed + speedBoost;
}

const float Ball::BASE_SPEED = 190;
const float Ball::SPEED_BOOST_DECAY_FACTOR = 0.5;