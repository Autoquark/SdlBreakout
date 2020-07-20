#include "stdafx.h"
#include "GameObject.h"
#include "game.h"

Texture* GameObject::getSprite()
{
	return sprite;
}

void GameObject::setSprite(Texture * value)
{
	sprite = value;
}

void GameObject::Update(float timeElapsed)
{
	SDL_Rect destinationRect;
	destinationRect.x = (int)collisionBounds.position.x;
	destinationRect.y = (int)collisionBounds.position.y;
	destinationRect.w = (int)sprite->GetSize().x;
	destinationRect.h = (int)sprite->GetSize().y;

	SDL_RenderCopy(Game::GetInstance().gRenderer, sprite->GetSdlTexture(), NULL, &destinationRect);
}

GameObject::GameObject(RectF collisionBounds) : collisionBounds(collisionBounds)
{
}


GameObject::~GameObject()
{
}
