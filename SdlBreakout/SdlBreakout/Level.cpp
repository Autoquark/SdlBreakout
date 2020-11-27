#include "stdafx.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Level.h"
#include "BlockMaker.h"
#include "Game.h"
#include "SerializableLevel.h"
#include "BallStatus_Accelerate.h"

std::unique_ptr<Level> Level::Load(std::filesystem::path path)
{
	std::ifstream file(path);
	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}

	nlohmann::json js = nlohmann::json::parse(file_contents);
	auto serializableLevel = js.get<SerializableLevel>();

	auto level = std::make_unique<Level>();

	// We only need one 'template' instance of each status that blocks can hold a pointer to
	// so it may as well be static local data in this function
	static std::map<std::string, const BallStatusEffect*> statusesByName = {
		{"accelerate", new BallStatus_Accelerate()}
	};

	auto invulnerable = Block("invulnerable");
	invulnerable.invulnerable = true;

	std::map<char, const Block> legend = {
		{'#', invulnerable}
	};

	for (const auto& entry : serializableLevel.legend)
	{
		auto block = Block(entry.invulnerable ? "invulnerable" : "normal");
		block.SetHealth(entry.health);
		if (!entry.effectName.empty())
		{
			block.appliesStatus = statusesByName.at(entry.effectName);
		}
		
		legend.insert({ entry.character, block });
	}

	float y = 0;//Game::SCREEN_HEIGHT;
	for (const auto& row : serializableLevel.grid)
	{
		float x = 0;
		for (const auto& character : row)
		{
			if (character == '.')
			{
				continue;
			}

			Block* block = NULL;
			if (isdigit(character))
			{
				block = BlockMaker::MakeNormal(character - '0');
			}
			else 
			{
				block = new Block(legend.at(character));
			}

			block->collisionBounds->Translate(x, y);
			level->AddBlock(block);
			x += Block::BLOCK_WIDTH;
		}
		y += Block::BLOCK_HEIGHT;
	}

	return level;
}


void Level::Destroy(GameObject* gameObject)
{
	blocks.erase(std::remove(blocks.begin(), blocks.end(), gameObject), blocks.end());
	balls.erase(std::remove(balls.begin(), balls.end(), gameObject), balls.end());
	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
	delete gameObject;
}

Level::Level() : bounds(AxisAlignedRectF(2.0f, 2.0f, Game::GetInstance().SCREEN_WIDTH - 4.0f, Game::GetInstance().SCREEN_HEIGHT - 4.0f))
{
	/*int x, y;
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
	}*/

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
