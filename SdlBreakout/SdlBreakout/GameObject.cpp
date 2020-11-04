#include "stdafx.h"

#include <typeinfo>
#include <typeindex>
#include "GameObject.h"
#include "game.h"
#include "Conversion.h"

const Texture* GameObject::getSprite()
{
	return sprite;
}

void GameObject::setSprite(Texture * value)
{
	sprite = value;
}

void GameObject::Update(float timeElapsed)
{
	if (collisionBounds == NULL || sprite == NULL)
	{
		return;
	}

	auto centre = collisionBounds->GetAxisAlignedBoundingBox().Centre();

	// Use the centre of the collision bounds but the size of the sprite, in case the sprite and collision bounds differ
	SDL_Rect destinationRect;
	destinationRect.x = (int)centre.x - sprite->GetSize().x / 2;
	destinationRect.y = (int)centre.y - sprite->GetSize().y / 2;
	destinationRect.w = (int)sprite->GetSize().x;
	destinationRect.h = (int)sprite->GetSize().y;

	auto& game = Game::GetInstance();
	SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
	SDL_RenderCopy(game.renderer, sprite->GetSdlTexture(), NULL, &destinationRect);

	if (game.drawCollisionShapes)
	{
		auto bounds = collisionBounds->GetAxisAlignedBoundingBox();
		auto rect = ToSdlShape(bounds);
		SDL_RenderDrawRectF(game.renderer, &rect);
	}
}