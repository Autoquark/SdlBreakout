#include "GameObject.h"
#include "game.h"

SDL_Texture * GameObject::getSprite()
{
	return sprite;
}

void GameObject::setSprite(SDL_Texture * value)
{
	sprite = value;
}

void GameObject::Update(float timeElapsed)
{
	SDL_Rect destinationRect;
	destinationRect.x = position.x;
	destinationRect.y = position.y;
	destinationRect.h = size.y;
	destinationRect.w = size.x;

	SDL_RenderCopy(Game::GetInstance()->gRenderer, sprite, NULL, &destinationRect);
	//SDL_BlitSurface(gBlockTexture, NULL, gScreenSurface, &destinationRect);
}

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}
