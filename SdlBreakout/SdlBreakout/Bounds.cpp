#include "stdafx.h"
#include "Game.h"
#include "Bounds.h"
#include "Textures.h"

Bounds::Bounds() : GameObject(new AxisAlignedRectF(2.0f, 2.0f, Game::GetInstance().SCREEN_WIDTH - 4.0f, Game::GetInstance().SCREEN_HEIGHT - 4.0f))
{
	sprite = Textures::GetTexture("bounds"s);
}
