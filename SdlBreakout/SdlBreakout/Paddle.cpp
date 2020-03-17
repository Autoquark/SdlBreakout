#include "Paddle.h"
#include "game.h"
#include <iostream>
#include <algorithm>

Paddle::Paddle() : GameObject(Vector2 {128, 32})
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
		position.x -= moveSpeed * timeElapsed;
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		position.x += moveSpeed * timeElapsed;
	}

	position.x = std::clamp(position.x, (float)0.0, (float)Game::GetInstance()->SCREEN_WIDTH);

	GameObject::Update(timeElapsed);

	std::cout << position.x << "," << position.y << '\n';
}