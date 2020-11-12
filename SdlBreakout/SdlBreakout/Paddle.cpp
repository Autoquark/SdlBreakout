#include "stdafx.h"

#include "Debug.h"
#include "Paddle.h"
#include "game.h"
#include "Textures.h"
#include "CompoundShape.h"
#include "CircleF.h"

#include <iostream>
#include <algorithm>

const float Paddle::MAX_VIRTUAL_CURVE = 20;

Paddle::Paddle() : GameObject(new CompoundShape(std::vector<Shape*>
{
	new CircleF(0, 16, 16),
	new AxisAlignedRectF(0, 0, 96, 32),
	new CircleF(96, 16, 16),
}))
{
	centreSegment = (AxisAlignedRectF*)((CompoundShape*)collisionBounds)->shapes[1];
	sprite = Textures::GetTexture("paddle");
	moveSpeed = 600;
}


Paddle::~Paddle()
{
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