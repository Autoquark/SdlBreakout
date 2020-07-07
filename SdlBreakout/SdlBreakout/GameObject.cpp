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
	destinationRect.x = (int)position.x;
	destinationRect.y = (int)position.y;
	destinationRect.h = (int)sprite->GetSize().x;
	destinationRect.w = (int)sprite->GetSize().y;

	SDL_RenderCopy(Game::GetInstance()->gRenderer, sprite->GetSdlTexture(), NULL, &destinationRect);
}

GameObject::GameObject(Vector2 size)
{
	this->size = size;
}


GameObject::~GameObject()
{
}
