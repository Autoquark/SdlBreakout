#include "block.h"
#include "Game.h"

Block::Block()
{
	sprite = Game::GetInstance()->gBlockTexture;
	size.x = 32;
	size.y = 16;
}


Block::~Block()
{
}