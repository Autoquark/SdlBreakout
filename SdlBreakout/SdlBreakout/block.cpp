#include "stdafx.h"
#include "Block.h"

#include <SDL_mixer.h>
#include <random>

#include "Game.h"
#include "Textures.h"
#include "Sounds.h"
#include "BallStatusEffect.h"
#include "PowerupDrop.h"
#include "Powerup.h"
#include "Powerup_Split.h"
#include "RenderUtils.h"

Block::Block(const std::string& textureKey, const Shape& collisionBounds) : GameObject(collisionBounds), textureKey(textureKey)
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
		auto& game = Game::GetInstance();
		auto* level = Game::GetLevel();
		level->ScheduleDestroy(this);
		if (std::uniform_real_distribution<double>(0, 1)(game.random) <= POWERUP_DROP_CHANCE)
		{
			level->AddGameObject(std::make_unique<PowerupDrop>(std::make_unique<Powerup_Split>()))
				->collisionBounds->SetCentre(collisionBounds->GetAxisAlignedBoundingBox().Centre());
		}
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
	RenderUtils::RenderTexture(Game::GetInstance().renderer, centre, Vector2F(0.5, 0.5), *statusSprite);
}

const float Block::POWERUP_DROP_CHANCE = 0.05f;
