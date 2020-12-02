#include "stdafx.h"
#include "Paddle.h"

#include <iostream>
#include <algorithm>

#include "Ball.h"
#include "Debug.h"
#include "Game.h"
#include "Textures.h"
#include "CompoundShape.h"
#include "CircleF.h"

#include "Util.h"

const float Paddle::MAX_VIRTUAL_CURVE = 20;

Paddle::Paddle()
{
	auto compound = std::make_unique<CompoundShape>(Util::MakeVector<std::unique_ptr<Shape>>(
		std::make_unique<CircleF>(0.0f, 16.0f, 16.0f),
		std::make_unique<AxisAlignedRectF>(0.0f, 0.0f, 96.0f, 32.0f),
		std::make_unique<CircleF>(96.0f, 16.0f, 16.0f)
		));
	centreSegment = static_cast<AxisAlignedRectF*>(compound->shapes[1].get());
	collisionBounds = std::move(compound);
	sprite = Textures::GetTexture("paddle");
	moveSpeed = 600;
}

void Paddle::Update(float timeElapsed)
{
	auto movement = Vector2F(std::clamp(Game::GetInput().GetMouseMovement().x, -moveSpeed * timeElapsed, moveSpeed * timeElapsed), 0);
	if (Game::GetInput().KeyIsDown(SDL_SCANCODE_LEFT))
	{
		movement = Vector2F(-moveSpeed * timeElapsed, 0);
	}
	else if (Game::GetInput().KeyIsDown(SDL_SCANCODE_RIGHT))
	{
		movement = Vector2F(moveSpeed * timeElapsed, 0);
	}

	auto level = Game::GetLevel();

	// Push balls as far as possible without passing the level bounds
	// Also keep track of the smallest contact distance to any of the (moved) balls
	std::optional<Contact> closestContact;
	for (auto ball : level->GetBalls())
	{
		auto ballContact = collisionBounds->CastAgainst(*ball->collisionBounds, movement, Shape::InternalityFilter::External);

		if (ballContact.has_value())
		{
			auto ballMovement = movement.WithMagnitude(movement.Magnitude() - ballContact->distance);
			ball->collisionBounds->MoveToContact(level->bounds, ballMovement);
			closestContact = Shape::ClosestContact(closestContact,
				collisionBounds->CastAgainst(*ball->collisionBounds, movement, Shape::InternalityFilter::External));
		}
	}

	auto bestContact = Shape::ClosestContact(closestContact, collisionBounds->CastAgainst(level->bounds, movement, Shape::InternalityFilter::Internal));
	// Move as far as possible without penetrating the ball or level bounds
	collisionBounds->MoveToContact(bestContact, movement);

	GameObject::Update(timeElapsed);
}