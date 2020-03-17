#include "block.h"
#include "Game.h"

Block::Block() : GameObject(Vector2{ 32, 16 })
{
	sprite = Game::GetInstance()->gBlockTexture;
}


Block::~Block()
{
}