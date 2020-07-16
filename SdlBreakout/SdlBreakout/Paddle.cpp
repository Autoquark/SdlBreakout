#include "stdafx.h"
#include "Paddle.h"
#include "game.h"
#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(RectF { 0, 0, 128, 32})
{
	sprite = Game::GetInstance()->gPaddleTexture;
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
		collisionBounds.x -= moveSpeed * timeElapsed;
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		collisionBounds.x += moveSpeed * timeElapsed;
	}

	collisionBounds.x = std::clamp(collisionBounds.x, (float)0.0, (float)Game::GetInstance()->SCREEN_WIDTH);

	GameObject::Update(timeElapsed);

	std::cout << collisionBounds.x << "," << collisionBounds.y << '\n';
}