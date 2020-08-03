#include "stdafx.h"
#include "Paddle.h"
#include "game.h"
#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(new AxisAlignedRectF{ 0, 0, 128, 32})
{
	sprite = Game::GetInstance().gPaddleTexture;
	moveSpeed = 600;
}


Paddle::~Paddle()
{
}

void Paddle::Update(float timeElapsed)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	Vector2F movement = Vector2F(0, 0);
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		movement = Vector2F(-moveSpeed * timeElapsed, 0);
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		movement = Vector2F(moveSpeed * timeElapsed, 0);
	}

	// Check if we're moving into the ball
	auto& ball = *Game::GetInstance().ball;
	auto ballHit = collisionBounds->CastAgainst(*ball.collisionBounds, movement, Shape::InternalityFilter::External);
	if (ballHit.has_value())
	{
		auto hit = ballHit.value();
		std::cout << "Paddle hit ball";
		// I think this is buggy because of numerical precision errors placing things in exact contact (ball ends up inside paddle)
		// OTOH sometimes there's not even an initial collision
		//collisionBounds->SetCentre(ballHit.value().centroid - movement.Scaled(0.5f));
		movement.Scale(std::max(hit.distance - 1.0f, 0.0f));
		collisionBounds->Translate(movement);

	}
	else
	{
		collisionBounds->Translate(movement);
	}

	// Clamp the position of the paddle to be fully inside the screen
	// Don't currently use a collision cast because if we're starting pressed up against the boundary numerical precision errors can let the paddle move past
	auto boundingBox = collisionBounds->GetAxisAlignedBoundingBox();
	if (boundingBox.Left() < 0)
	{
		collisionBounds->Translate(-boundingBox.Left(), 0);
	}
	else if (boundingBox.Right() > Game::GetInstance().SCREEN_WIDTH)
	{
		collisionBounds->Translate(Game::GetInstance().SCREEN_WIDTH - boundingBox.Right(), 0);
	}

	GameObject::Update(timeElapsed);
}