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

	const std::vector<Block*>& GetBlocks() const
	{
		return blocks;
	}

	const std::vector<Ball*>& GetBalls() const
	{
		return balls;
	}

	void AddBlock(Block* block)
	{
		blocks.push_back(block);
		gameObjects.push_back(block);
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

	std::vector<GameObject*> gameObjects;
	std::vector<Block*> blocks;
	std::vector<Ball*> balls;
	std::unordered_set<GameObject*> toRemove;
	int lives = 3;
};

