#include "stdafx.h"
#include "PowerupDrop.h"

#include "Game.h"
#include "Level.h"
#include "Paddle.h"

void PowerupDrop::Update(float timeElapsed)
{
	yVelocity = std::min(yVelocity + ACCELERATION * timeElapsed, MAX_SPEED);

	auto* level = Game::GetLevel();

	auto movement = Vector2F(0, yVelocity * timeElapsed);
	if (collisionBounds->CastAgainst(*level->paddle->collisionBounds.get(), movement, Shape::InternalityFilter::External).has_value())
	{
		Collect();
	}
	else if (!Game::levelArea.Overlaps(collisionBounds->GetAxisAlignedBoundingBox()))
	{
		level->ScheduleDestroy(this);
	}
	else
	{
		collisionBounds->Translate(movement);
	}

	GameObject::Update(timeElapsed);
}

void PowerupDrop::Collect()
{
	auto* level = Game::GetLevel();
	level->paddle->SetPowerup(std::move(powerup));
	level->ScheduleDestroy(this);
}

const float PowerupDrop::ACCELERATION = 50;
const float PowerupDrop::MAX_SPEED = 190;
