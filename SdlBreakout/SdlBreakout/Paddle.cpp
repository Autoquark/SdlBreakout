#include "stdafx.h"
#include "Paddle.h"
#include "game.h"
#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(AxisAlignedRectF{ 0, 0, 128, 32})
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
		collisionBounds.position.x -= moveSpeed * timeElapsed;
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		collisionBounds.position.x += moveSpeed * timeElapsed;
	}

	collisionBounds.position.x = std::clamp(collisionBounds.position.x, 0.0f, (float)Game::GetInstance().SCREEN_WIDTH - collisionBounds.size.x);

	GameObject::Update(timeElapsed);
}