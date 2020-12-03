#include "stdafx.h"
#include "Block.h"

#include <SDL_mixer.h>

#include "Game.h"
#include "Textures.h"
#include "Sounds.h"
#include "BallStatusEffect.h"

Block::Block(const std::string& textureKey, const Shape& collisionBounds) : GameObject(&collisionBounds), textureKey(textureKey)
{
	SetHealth(1);
}


Block::~Block()
{
}

void Block::OnBallHit(Ball& ball)
{
	if (appliesStatus)
	{
		appliesStatus->ApplyToBall(&ball);
	}

	if (!invulnerable)
	{
		SetHealth(health - 1);
		Mix_PlayChannel(-1, Sounds::GetSound("bounce"), 0);
	}
	else
	{
		Mix_PlayChannel(-1, Sounds::GetSound("bounce_invulnerable"), 0);
	}
}

void Block::SetHealth(int value)
{
	health = value;
	if (health <= 0)
	{
		Game::GetLevel()->ScheduleDestroy(this);
		return;
	}
	sprite = Textures::GetBlockTextures(textureKey)[((size_t)health) - 1];
}

void Block::Update(float timeElapsed)
{
	GameObject::Update(timeElapsed);

	if (!appliesStatus)
	{
		return;
	}

	auto centre = collisionBounds->GetAxisAlignedBoundingBox().Centre();
	auto statusSprite = appliesStatus->GetIcon();
	SDL_Rect destinationRect{};
	destinationRect.x = (int)centre.x - statusSprite->size.x / 2;
	destinationRect.y = (int)centre.y - statusSprite->size.y / 2;
	destinationRect.w = (int)statusSprite->size.x;
	destinationRect.h = (int)statusSprite->size.y;

	auto& game = Game::GetInstance();
	SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
	SDL_RenderCopy(game.renderer, statusSprite->GetSdlTexture(), NULL, &destinationRect);
}
