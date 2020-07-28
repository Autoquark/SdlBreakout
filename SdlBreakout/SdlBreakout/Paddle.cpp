#include "stdafx.h"
#include "Paddle.h"
#include "game.h"
#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(new AxisAlignedRectF{ 0, 0, 128, 32})
{
	sprite = Game::GetInstance().gPaddleTexture;
	moveSpeed = 1000;
}


Paddle::~Paddle()
{
}

void Paddle::Update(float timeElapsed)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		collisionBounds->Translate(-moveSpeed * timeElapsed, 0);
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		collisionBounds->Translate(moveSpeed * timeElapsed, 0);
	}

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