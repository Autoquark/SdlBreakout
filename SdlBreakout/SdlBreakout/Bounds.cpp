#include "stdafx.h"
#include "Bounds.h"

#include "Game.h"
#include "Textures.h"

Bounds::Bounds() : GameObject(new AxisAlignedRectF(2.0f, 2.0f, Game::SCREEN_WIDTH - 4.0f, Game::SCREEN_HEIGHT - 4.0f))
{
	sprite = Textures::GetTexture("bounds"s);
}
