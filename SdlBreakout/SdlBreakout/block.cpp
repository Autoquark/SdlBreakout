#include "stdafx.h"

#include <SDL_mixer.h>

#include "block.h"
#include "Game.h"
#include "Textures.h"
#include "Sounds.h"

Block::Block() : GameObject(new AxisAlignedRectF{ 0, 0, 32, 16 })
{
	sprite = Textures::GetTexture("block"s);
}


Block::~Block()
{
}

void Block::OnBallHit(Ball& ball)
{
	Mix_PlayChannel(-1, Sounds::GetSound("bounce"), 0);
	Game::GetInstance().Destroy(this);
}
