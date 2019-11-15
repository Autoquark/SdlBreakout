#include "GameObject.h"
#include "main.h"

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
	destinationRect.h = 32;
	destinationRect.w = 32;

	SDL_RenderCopy(gRenderer, sprite, NULL, &destinationRect);
	//SDL_BlitSurface(gBlockTexture, NULL, gScreenSurface, &destinationRect);
}

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}
