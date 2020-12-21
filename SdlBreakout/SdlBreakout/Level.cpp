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
#include "PowerupDrop.h"

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
				level->AddGameObject(std::move(block));
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

	auto ball = AddGameObject(std::make_unique<Ball>());
	ball->collisionBounds->SetCentre(paddle->collisionBounds->GetAxisAlignedBoundingBox().Centre().x, 0);
	ball->collisionBounds->MoveToContact(*paddle->collisionBounds, Vector2F(0, 999), Shape::InternalityFilter::External);
}

Level::Level()
{
	auto unique = std::make_unique<Paddle>();
	paddle = unique.get();
	gameObjects.push_back(std::move(unique));
	gameObjects.back()->collisionBounds->SetCentre(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT - paddle->collisionBounds->GetAxisAlignedBoundingBox().size.y / 2) - 1);
	
	SpawnBallAndStickToPaddle();
}

Level::~Level() = default;

template<typename T>
void Level::AddPendingGameObjects(Level::GameObjectsInfo<T>& info)
{
	for (auto& gameObject : info.toAdd)
	{
		auto pointer = gameObject.get();
		gameObjects.push_back(std::move(gameObject));
		info.current.push_back(pointer);
	}

	info.toAdd.clear();
}

template<typename T>
void Level::RemoveDestroyedGameObject(Level::GameObjectsInfo<T>& info, GameObject* object)
{
	info.current.erase(std::remove(info.current.begin(), info.current.end(), object), info.current.end());
}

Level::UpdateResult Level::Update(float timeElapsed)
{
	if (levelStartTime < 0)
	{
		levelStartTime = Game::GetInstance().GetTime();
	}

	auto& game = Game::GetInstance();

	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(timeElapsed);
	}

	// Handle pending GameObject destructions
	for (auto gameObject : toRemove)
	{
		// Here we are defining a lambda which takes a parameter pack. We then call RemoveDestroyedGameObject<T> for each parameter. We do this via a fold expression using the comma operator,
		// (the comma operator evaluates both expressions and returns the RHS). This means that the lambda must actually be a varadic template (how else could the compiler know the template 
		// parameter for each invocation of RemoveDestroyedGameObject<T>?). We then pass it to apply along with our tuple. Apply turns the tuple elements into a parameter pack.
		std::apply([&](auto&... infos)
		{
			(RemoveDestroyedGameObject(infos, gameObject), ...);
		}, gameObjectsInfo);

		gameObjects.erase(std::find_if(gameObjects.begin(), gameObjects.end(), [&](auto& ptr) { return ptr.get() == gameObject; }));
	}
	toRemove.clear();

	// Game objects of tracked types to add
	std::apply([&](auto&... infos)
	{
		(AddPendingGameObjects(infos), ...);
	}, gameObjectsInfo);

	// Plain game objects to add
	for (auto& gameObject : gameObjectsToAdd)
	{
		gameObjects.push_back(std::move(gameObject));
	}
	gameObjectsToAdd.clear();

	if (GetBlocks().empty())
	{
		return UpdateResult::Victory;
	}
	else if (GetBalls().empty())
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

