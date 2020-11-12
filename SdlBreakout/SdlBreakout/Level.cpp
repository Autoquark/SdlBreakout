#include "stdafx.h"
#include "Level.h"

#include "BlockMaker.h"
#include "Game.h"

void Level::Destroy(GameObject* gameObject)
{
	blocks.erase(std::find(blocks.begin(), blocks.end(), gameObject));
	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
	delete gameObject;
}

Level::Level()
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

	ball = new Ball();
	gameObjects.push_back(ball);
	gameObjects.back()->collisionBounds->Translate(320, 400);

	bounds = new Bounds();
	gameObjects.push_back(bounds);
}

void Level::Update(float timeElapsed)
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
}
