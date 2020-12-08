#include "stdafx.h"
#include "GameObject.h"

#include "Game.h"
#include "Conversion.h"
#include "RenderUtils.h"

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
	RenderUtils::RenderTexture(Game::GetInstance().renderer, centre, Vector2F(0.5, 0.5), *sprite);

	auto& game = Game::GetInstance();
	if (game.drawCollisionShapes)
	{
		auto bounds = collisionBounds->GetAxisAlignedBoundingBox();
		auto rect = ToSdlShape(bounds);
		SDL_RenderDrawRectF(game.renderer, &rect);
	}
}