#include "stdafx.h"
#include "block.h"
#include "Game.h"

Block::Block() : GameObject(RectF{ 0, 0, 32, 16 })
{
	sprite = Game::GetInstance()->gBlockTexture;
}


Block::~Block()
{
}