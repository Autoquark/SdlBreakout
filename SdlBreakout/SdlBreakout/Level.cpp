#include "stdafx.h"
#include "Level.h"

#include "BlockMaker.h"
#include "Game.h"

void Level::Destroy(GameObject* gameObject)
{
	blocks.erase(std::remove(blocks.begin(), blocks.end(), gameObject), blocks.end());
	balls.erase(std::remove(balls.begin(), balls.end(), gameObject), balls.end());
	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
	delete gameObject;
}

Level::Level() : bounds(AxisAlignedRectF(2.0f, 2.0f, Game::GetInstance().SCREEN_WIDTH - 4.0f, Game::GetInstance().SCREEN_HEIGHT - 4.0f))
{
	int x, y;
	for (y = 120; y < 340; y += (int)Textures::GetTexture("block")->GetSize().y)
	{
		for (x = 240; x < 480; x += (int)Textures::GetTexture("block")->GetSize().x)
		{
			blocks.push_back(BlockMaker::MakeNormal(3));
			blocks.back()->collisionBounds->Translate((float)x, (float)y);
		}
	}

	for (x = 240; x < 480; x += (int)Textures::GetTexture("block")->GetSize().x)
	{
		blocks.push_back(BlockMaker::MakeInvulnerable());
		blocks.back()->collisionBounds->Translate((float)x, (float)y);
	}

	for (auto block : blocks)
	{
		gameObjects.push_back(block);
	}

	paddle = new Paddle();
	gameObjects.push_back(paddle);
	gameObjects.back()->collisionBounds->Translate(320, 440);

	balls.push_back(new Ball());
	gameObjects.push_back(balls.back());
	gameObjects.back()->collisionBounds->Translate(320, 400);
}

Level::UpdateResult Level::Update(float timeElapsed)
{
	for (auto gameObject : gameObjects)
	{
		gameObject->Update(timeElapsed);
	}

	// Debug tool: Speed up when space held
	if (Game::GetInput().KeyIsDown(SDL_SCANCODE_SPACE))
	{
		for (auto gameObject : gameObjects)
		{
			gameObject->Update(timeElapsed);
		}
	}

	if (balls.empty())
	{
		return UpdateResult::Defeat;
	}

	return UpdateResult::Continue;
}
