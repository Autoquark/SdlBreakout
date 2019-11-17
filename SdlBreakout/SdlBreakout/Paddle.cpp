#include "Paddle.h"
#include "main.h"


Paddle::Paddle()
{
	sprite = gPaddleTexture;
	moveSpeed = 100;
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

	GameObject::Update(timeElapsed);
}