#pragma once
#include "stdafx.h"

#include "Paddle.h"
#include "Bounds.h"
#include "Ball.h"

class Level
{
public:

	Level();
	Level(const Level& level) = delete;
	Level(int x) = delete;

	void Update(float timeElapsed);

	const std::vector<Block*>& GetBlocks() const
	{
		return blocks;
	}

	/// <summary>
	/// Destroys the given game object. After calling this method, pointers to the object are no longer valid.
	/// GameObjects should not be deleted except via this method
	/// </summary>
	/// <param name="gameObject"></param>
	void Destroy(GameObject* gameObject);

	// Special game objects
	Bounds* bounds = NULL;
	Paddle* paddle = NULL;
	Ball* ball = NULL;

private:

	std::vector<GameObject*> gameObjects;
	std::vector<Block*> blocks;
};

