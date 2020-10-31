#include "stdafx.h"

#include <SDL_mixer.h>

#include "block.h"
#include "Game.h"
#include "Textures.h"
#include "Sounds.h"

Block::Block(std::string textureKey) : GameObject(new AxisAlignedRectF{ 0, 0, 40, 20 }), textureKey(textureKey)
{
	SetHealth(1);
}


Block::~Block()
{
}

void Block::OnBallHit(Ball& ball)
{
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
	if (health == 0)
	{
		Game::GetInstance().Destroy(this);
		return;
	}
	sprite = Textures::GetBlockTextures(textureKey)[health - 1];
}
