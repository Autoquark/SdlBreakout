#pragma once
#include "stdafx.h"

#include <unordered_set>
#include <filesystem>

#include "Block.h"

class Paddle;
class Ball;

class Level
{
public:
	static std::unique_ptr<Level> Load(std::filesystem::path path);

	Level();
	Level(const Level& level) = delete;
	Level(const Level&& level) = delete;

	Level& operator=(const Level&) = delete;

	enum class UpdateResult
	{
		Continue,
		Defeat,
		Victory
	};

	UpdateResult Update(float timeElapsed);

	float GetStartTime() const
	{
		return levelStartTime;
	}

	const std::vector<Block*>& GetBlocks() const
	{
		return blocks;
	}

	const std::vector<Ball*>& GetBalls() const
	{
		return balls;
	}

	void AddBlock(std::unique_ptr<Block> block)
	{
		blocks.push_back(block.get());
		gameObjects.push_back(std::move(block));
	}

	GameObject* AddGameObject(std::unique_ptr<GameObject> gameObject)
	{
		gameObjectsToAdd.push_back(std::move(gameObject));
		return gameObjectsToAdd.back().get();
	}

	/// <summary>
	/// Schedules the given game object for destruction at the end of the update. GameObjects should not be deleted except via this method
	/// </summary>
	/// <param name="gameObject"></param>
	void ScheduleDestroy(GameObject* gameObject);

	// Special game objects
	Paddle* paddle = NULL;

private:

	void SpawnBallAndStickToPaddle();

	// The game time in milliseconds when the level was started
	float levelStartTime = -1;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<GameObject>> gameObjectsToAdd;
	std::vector<Block*> blocks;
	std::vector<Ball*> balls;
	std::unordered_set<GameObject*> toRemove;
};

