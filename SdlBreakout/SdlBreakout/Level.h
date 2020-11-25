#pragma once
#include "stdafx.h"

#include "Paddle.h"
#include "Bounds.h"
#include "Ball.h"

class Level
{
public:
	static Level Load(std::filesystem::path path);

	Level();
	Level(const Level& level) = delete;
	Level(const Level&& level) = delete;

	enum class UpdateResult
	{
		Continue,
		Defeat
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

	/// <summary>
	/// Destroys the given game object. After calling this method, pointers to the object are no longer valid.
	/// GameObjects should not be deleted except via this method
	/// </summary>
	/// <param name="gameObject"></param>
	void Destroy(GameObject* gameObject);

	AxisAlignedRectF bounds;

	// Special game objects
	Paddle* paddle = NULL;

private:

	std::vector<GameObject*> gameObjects;
	std::vector<Block*> blocks;
	std::vector<Ball*> balls;
};

