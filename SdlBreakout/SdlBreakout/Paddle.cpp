#include "stdafx.h"

#include "Debug.h"
#include "Paddle.h"
#include "game.h"
#include "Textures.h"
#include "CompoundShape.h"

#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(new CompoundShape(std::vector<Shape*>{new AxisAlignedRectF{ 0, 0, 128, 32 }}))
{
	sprite = Textures::GetTexture("paddle");
	moveSpeed = 600;
}


Paddle::~Paddle()
{
}

void Paddle::Update(float timeElapsed)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	Vector2F movement = Vector2F(0, 0);
	if (currentKeyStates[SDL_SCANCODE_LEFT] && !blockedLeft)
	{
		movement = Vector2F(-moveSpeed * timeElapsed, 0);
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT] && !blockedRight)
	{
		movement = Vector2F(moveSpeed * timeElapsed, 0);
	}

	auto& game = Game::GetInstance();
	// Check if we're moving into the ball
	auto& ball = *Game::GetInstance().ball;
	auto bestContact = collisionBounds->CastAgainst(*ball.collisionBounds, movement, Shape::InternalityFilter::External);
	if (movement.x != 0 && collisionBounds->GetAxisAlignedBoundingBox().Left() + movement.x <= 2)
	{
		auto x = 1;
	}
	bestContact = Shape::ClosestContact(bestContact, collisionBounds->CastAgainst(*game.bounds->collisionBounds, movement, Shape::InternalityFilter::Internal));
	if (bestContact.has_value())
	{
		auto hit = bestContact.value();
		std::cout << "Paddle hit something\n";
		// I think this is buggy because of numerical precision errors placing things in exact contact (ball ends up inside paddle)
		// OTOH sometimes there's not even an initial collision
		//collisionBounds->SetCentre(bestContact.value().centroid - movement.WithLength(0.5f));
		movement.SetLength(std::max(hit.distance - 1.0f, 0.0f));
		collisionBounds->Translate(movement);
		std::cout << "Paddle moving " << movement.x << std::endl;
	}
	else
	{
		collisionBounds->Translate(movement);
	}

	Debug::PrintChanges("paddle", collisionBounds->GetAxisAlignedBoundingBox().Left());

	// Clamp the position of the paddle to be fully inside the screen
	// Don't currently use a collision cast because if we're starting pressed up against the boundary numerical precision errors can let the paddle move past
	
	

	/*auto boundingBox = collisionBounds->GetAxisAlignedBoundingBox();
	if (boundingBox.Left() < game.bounds->collisionBounds->GetAxisAlignedBoundingBox().Left())
	{
		collisionBounds->Translate(-boundingBox.Left(), 0);
	}
	else if (boundingBox.Right() > Game::GetInstance().SCREEN_WIDTH)
	{
		collisionBounds->Translate(Game::GetInstance().SCREEN_WIDTH - boundingBox.Right(), 0);
	}*/

	GameObject::Update(timeElapsed);
}