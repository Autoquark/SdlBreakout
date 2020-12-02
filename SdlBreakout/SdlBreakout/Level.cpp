#include "stdafx.h"
#include "Level.h"

#include <fstream>
#include <nlohmann/json.hpp>

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
		
		legend.insert({ entry.character[0], block });
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
	toRemove.insert(gameObject);
}

Level::Level() : bounds(AxisAlignedRectF(2.0f, 2.0f, Game::GetInstance().SCREEN_WIDTH - 4.0f, Game::GetInstance().SCREEN_HEIGHT - 4.0f))
{
	paddle = new Paddle();
	gameObjects.push_back(paddle);
	gameObjects.back()->collisionBounds->SetCentre(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT - paddle->centreSegment->size.y / 2);
	paddle->isSticky = true;

	balls.push_back(new Ball());
	gameObjects.push_back(balls.back());
	gameObjects.back()->collisionBounds->Translate(320, 0);
	gameObjects.back()->collisionBounds->MoveToContact(*paddle->collisionBounds.get(), Vector2F(0, 999), Shape::InternalityFilter::External);
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

	for (auto gameObject : toRemove)
	{
		blocks.erase(std::remove(blocks.begin(), blocks.end(), gameObject), blocks.end());
		balls.erase(std::remove(balls.begin(), balls.end(), gameObject), balls.end());
		gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
		delete gameObject;
	}
	toRemove.clear();

	if (balls.empty())
	{
		return UpdateResult::Defeat;
	}

	return UpdateResult::Continue;
}
