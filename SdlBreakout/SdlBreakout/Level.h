#pragma once
#include "stdafx.h"

#include <unordered_set>
#include <filesystem>

#include "Block.h"
#include "PowerupDrop.h"

class Paddle;

class Level
{
public:
	static std::unique_ptr<Level> Load(std::filesystem::path path);

	Level();
	Level(const Level& level) = delete;
	Level(const Level&& level) = delete;
	~Level();

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
		return GetGameObjects<Block>();
	}

	const std::vector<Ball*>& GetBalls() const
	{
		return GetGameObjects<Ball>();
	}

	// Gets all game objects of a tracked type T
	template<typename T>
	const std::vector<T*>& GetGameObjects() const
	{
		return std::get<GameObjectsInfo<T>>(gameObjectsInfo).current;
	}

	// Adds a game object of a tracked type T to the level
	template<typename T>
	T* AddGameObject(std::unique_ptr<T> gameObject)
	{
		auto pointer = gameObject.get();
		std::get<GameObjectsInfo<T>>(gameObjectsInfo).toAdd.push_back(std::move(gameObject));
		return pointer;
	}

	/// <summary>
	/// Schedules the given game object for destruction at the end of the update. GameObjects should not be deleted except via this method
	/// </summary>
	/// <param name="gameObject"></param>
	void ScheduleDestroy(GameObject* gameObject);

	// Special game objects
	Paddle* paddle = NULL;

private:

	template<typename T>
	struct GameObjectsInfo
	{
		std::vector<T*> current;
		std::vector<std::unique_ptr<T>> toAdd;
	};

	template<typename T>
	void AddPendingGameObjects(GameObjectsInfo<T>&);

	template<typename T>
	void RemoveDestroyedGameObject(Level::GameObjectsInfo<T>&, GameObject*);

	void SpawnBallAndStickToPaddle();

	// The game time in milliseconds when the level was started
	float levelStartTime = -1;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<GameObject>> gameObjectsToAdd;
	std::unordered_set<GameObject*> toRemove;

	std::tuple<GameObjectsInfo<Ball>, GameObjectsInfo<Block>, GameObjectsInfo<PowerupDrop>> gameObjectsInfo;
};

