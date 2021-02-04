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
#include "CompoundShape.h"

Ball::Ball() : GameObject(CircleF(0, 0, 8))
{
	sprite = Textures::GetTexture("ball"s);
}

Ball::Ball(const Ball& ball) : GameObject(ball), firstReleasedTime(ball.firstReleasedTime), speedBoost(ball.speedBoost), hitChain(ball.hitChain), direction(ball.direction)
{
	for (auto& status : ball.statusEffects)
	{
		status->ApplyToBall(this);
		if (status.get() == ball.heldStatus)
		{
			heldStatus = static_cast<BallStatus_PaddleHeld*>(statusEffects.back().get());
		}
	}
}


void Ball::Update(float timeElapsed)
{
	auto& game = Game::GetInstance();

	auto level = Game::GetLevel();
	auto paddle = level->paddle;
	if (Game::GetInput().MouseButtonPressed(Input::MouseButton::Left) && heldStatus != nullptr)
	{
		if (firstReleasedTime < 0)
		{
			firstReleasedTime = Game::GetInstance().GetTime();
		}
		RemoveStatus(heldStatus);
		heldStatus = nullptr;
		paddle->isSticky = false;
	}

	auto effectiveSpeed = CalculateEffectiveSpeed();

	auto remainingVelocity = direction * effectiveSpeed * timeElapsed;
	if (remainingVelocity.Magnitude() > 0)
	{
		while (true)
		{
			auto castVector = remainingVelocity.WithMagnitude(remainingVelocity.Magnitude() + Contact::MIN_SEPARATION_DISTANCE);

			auto screenEdgeCollision = collisionBounds->CastAgainst(Game::levelArea, castVector, Shape::InternalityFilter::Internal);
			auto paddleCollision = collisionBounds->CastAgainst(*paddle->collisionBounds, castVector, Shape::InternalityFilter::External);

			std::vector<std::optional<Contact>> contacts = {
				screenEdgeCollision,
				paddleCollision
			};

			auto& blocks = level->GetBlocks();
			std::transform(blocks.begin(), blocks.end(),
				std::back_inserter(contacts),
				[&](auto const block) { return collisionBounds->CastAgainst(*block->collisionBounds, castVector, Shape::InternalityFilter::External); });

			auto index = Shape::FindClosestCollisionIndex(contacts);
			if (index == -1)
			{
				collisionBounds->Translate(remainingVelocity);
				break;
			}

			auto& collision = *contacts[index];
			std::optional<Vector2F> overrideDirection;

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
					hitChain++;
					game.score += 10 * hitChain;
				}
			}
			else if (!Game::DISABLE_BALL_LOSS && index == 0 && std::abs(collision.point.y - Game::levelArea.Bottom()) < 1)
			{
				level->ScheduleDestroy(this);
				return;
			}
			// If we hit the paddle
			else if (index == 1)
			{
				hitChain = 0;
				if (paddle->isSticky)
				{
					collisionBounds->MoveToContact(collision);

					auto unique = std::make_unique<BallStatus_PaddleHeld>();
					heldStatus = unique.get();
					AddStatus(std::move(unique));
					break;
				}
				else
				{
					//auto centreSegment = static_cast<CompoundShape*>(level->paddle->collisionBounds.get())->shapes[1].get();
					//auto contact = collisionBounds->CastAgainst(*centreSegment, castVector, Shape::InternalityFilter::External);
					auto contact = std::optional(collision);
					if (contact.has_value() && contact.value().distance <= collision.distance)
					{
						// We need to negate the value here because in SDL +y is down
						auto curveProportion = -(collision.point.x - paddle->collisionBounds->GetCentre().x) * 2 / paddle->collisionBounds->GetAxisAlignedBoundingBox().size.x;
						overrideDirection = Vector2F::Up().Rotated(curveProportion * Paddle::MAX_VIRTUAL_CURVE);

						auto paddleSpeed = paddle->GetSpeedLastUpdate();
						auto dotProduct = paddleSpeed.DotProduct(contact->normal);
						if (dotProduct > 0)
						{
							overrideDirection = Vector2F::LinearInterpolate(overrideDirection.value(), contact->normal, dotProduct / (dotProduct + effectiveSpeed));
							speedBoost += dotProduct;
						}
					}
				}
			}

			auto actualDistance = collisionBounds->MoveToContact(collision).Magnitude();
			remainingVelocity *= 1 - (actualDistance / remainingVelocity.Magnitude());

			if (overrideDirection.has_value())
			{
				remainingVelocity.SetDirection(*overrideDirection);
				direction.SetDirection(*overrideDirection);
			}
			else
			{
				remainingVelocity.Reflect(collision.normal);
				direction.Reflect(collision.normal);
			}
		}
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
	auto effectiveSpeed = BASE_SPEED * (1 + ((Game::GetInstance().GetTime() - firstReleasedTime) * MAX_SPEED_MULTIPLIER / TIME_UNTIL_MAX_SPEED));
	
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

// Initial speed in pixels per second
const float Ball::BASE_SPEED = 190;
// The proportion of the speed boost value that will remain after one second of decay
const float Ball::SPEED_BOOST_DECAY_FACTOR = 0.5;
// The time needed for extra speed to reach MAX_SPEED_MULTIPLIER
const float Ball::TIME_UNTIL_MAX_SPEED = 180;
// The maximum additional speed that the ball can attain, as a multiple of BASE_SPEED
const float Ball::MAX_SPEED_MULTIPLIER = 2;