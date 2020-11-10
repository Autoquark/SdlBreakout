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
	Vector2F movement = Vector2F(0, 0);
	if (Game::GetInput().KeyIsDown(SDL_SCANCODE_LEFT))
	{
		movement = Vector2F(-moveSpeed * timeElapsed, 0);
	}
	else if (Game::GetInput().KeyIsDown(SDL_SCANCODE_RIGHT))
	{
		movement = Vector2F(moveSpeed * timeElapsed, 0);
	}

	auto& game = Game::GetInstance();

	// Check if we're moving into the ball
	auto& ball = *Game::GetInstance().ball;
	auto ballContact = collisionBounds->CastAgainst(*ball.collisionBounds, movement, Shape::InternalityFilter::External);

	if (ballContact.has_value())
	{
		// Push the ball as far as possible without passing the level bounds
		auto ballMovement = movement.WithMagnitude(movement.Magnitude() - ballContact->distance);
		ball.collisionBounds->MoveToContact(*game.bounds->collisionBounds, ballMovement);
	}

	auto bestContact = collisionBounds->CastAgainst(*ball.collisionBounds, movement, Shape::InternalityFilter::External);
	bestContact = Shape::ClosestContact(bestContact, collisionBounds->CastAgainst(*game.bounds->collisionBounds, movement, Shape::InternalityFilter::Internal));
	// Move as far as possible without penetrating the ball or level bounds
	collisionBounds->MoveToContact(bestContact, movement);

	GameObject::Update(timeElapsed);
}