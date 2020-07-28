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
	auto centre = collisionBounds->GetAxisAlignedBoundingBox().Centre();

	// Use the centre of the collision bounds but the size of the sprite, in case the sprite and collision bounds differ
	SDL_Rect destinationRect;
	destinationRect.x = (int)centre.x - sprite->GetSize().x / 2;
	destinationRect.y = (int)centre.y - sprite->GetSize().y / 2;
	destinationRect.w = (int)sprite->GetSize().x;
	destinationRect.h = (int)sprite->GetSize().y;

	SDL_RenderCopy(Game::GetInstance().gRenderer, sprite->GetSdlTexture(), NULL, &destinationRect);
}

GameObject::GameObject(AxisAlignedRectF* collisionBounds) : collisionBounds(collisionBounds)
{
}