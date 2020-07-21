#include "stdafx.h"
#include "block.h"
#include "Game.h"

Block::Block() : GameObject(AxisAlignedRectF{ 0, 0, 32, 16 })
{
	sprite = Game::GetInstance().gBlockTexture;
}


Block::~Block()
{
}

void Block::OnBallHit(Ball& ball)
{
	Game::GetInstance().Destroy(this);
}
