#include "stdafx.h"
#include "Level.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <SDL_ttf.h>

#include "Fonts.h"
#include "BlockMaker.h"
#include "Game.h"
#include "SerializableLevel.h"
#include "BallStatus_Accelerate.h"
#include "RenderUtils.h"

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

	float y = Game::levelArea.Top();
	for (const auto& row : serializableLevel.grid)
	{
		float x = Game::levelArea.Left();
		for (const auto& character : row)
		{
			if (character != '.')
			{
				std::unique_ptr<Block> block;
				if (isdigit(character))
				{
					block = BlockMaker::MakeNormal(character - '0');
				}
				else
				{
					block = std::make_unique<Block>(legend.at(character));
				}

				block->collisionBounds->Translate(x, y);
				level->AddBlock(std::move(block));
			}

			x += Block::BLOCK_WIDTH;
			if (x > Game::levelArea.Right())
			{
				throw new std::exception();
			}
		}

		y += Block::BLOCK_HEIGHT;
		if (y > Game::levelArea.Bottom())
		{
			throw new std::exception();
		}
	}

	return level;
}


void Level::ScheduleDestroy(GameObject* gameObject)
{
	toRemove.insert(gameObject);
}

void Level::SpawnBallAndStickToPaddle()
{
	paddle->isSticky = true;

	auto unique = std::make_unique<Ball>();
	balls.push_back(unique.get());
	gameObjects.push_back(std::move(unique));
	gameObjects.back()->collisionBounds->SetCentre(paddle->collisionBounds->GetAxisAlignedBoundingBox().Centre().x, 0);
	gameObjects.back()->collisionBounds->MoveToContact(*paddle->collisionBounds, Vector2F(0, 999), Shape::InternalityFilter::External);
}

Level::Level()
{
	auto unique = std::make_unique<Paddle>();
	paddle = unique.get();
	gameObjects.push_back(std::move(unique));
	gameObjects.back()->collisionBounds->SetCentre(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT - paddle->centreSegment->size.y / 2);
	
	SpawnBallAndStickToPaddle();
}

Level::UpdateResult Level::Update(float timeElapsed)
{
	auto& game = Game::GetInstance();
	RenderUi();

	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(timeElapsed);
	}

	// Debug tool: Speed up when space held
	if (Game::GetInput().KeyIsDown(SDL_SCANCODE_SPACE))
	{
		for (auto& gameObject : gameObjects)
		{
			gameObject->Update(timeElapsed);
		}
	}

	for (auto gameObject : toRemove)
	{
		blocks.erase(std::remove(blocks.begin(), blocks.end(), gameObject), blocks.end());
		balls.erase(std::remove(balls.begin(), balls.end(), gameObject), balls.end());
		gameObjects.erase(std::find_if(gameObjects.begin(), gameObjects.end(), [&](auto& ptr) { return ptr.get() == gameObject; }));
	}
	toRemove.clear();

	if (blocks.empty())
	{
		return UpdateResult::Victory;
	}
	else if (balls.empty())
	{
		game.lives--;
		if (game.lives == 0)
		{
			return UpdateResult::Defeat;
		}

		SpawnBallAndStickToPaddle();
	}

	return UpdateResult::Continue;
}

void Level::RenderUi()
{
	auto& game = Game::GetInstance();
	const auto* sprite = Textures::GetTexture("life");

	Vector2 position(0, 0);
	for (int i = 0; i < game.lives; i++)
	{
		SDL_Rect destinationRect{};
		destinationRect.x = position.x;
		destinationRect.y = position.y;
		destinationRect.w = (int)sprite->size.x;
		destinationRect.h = (int)sprite->size.y;

		SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
		SDL_RenderCopy(game.renderer, sprite->GetSdlTexture(), NULL, &destinationRect);

		position.x += (int)(sprite->size.x * 1.5);
	}

	RenderUtils::RenderText(game.renderer, Vector2<int>(Game::SCREEN_WIDTH, 0), Vector2F(1, 0), std::to_string(game.score), Fonts::menuFont, SDL_Color{ 255, 255, 255 }, true);
}