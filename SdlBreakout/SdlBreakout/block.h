#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Ball.h"

#pragma once
class Block : public GameObject
{
public:
	static const int BLOCK_WIDTH = 40;
	static const int BLOCK_HEIGHT = 20;

	Block(std::string textureKey, Shape& collisionBounds);
	Block(std::string textureKey) : Block(textureKey, *new AxisAlignedRectF{ 0, 0, (float)BLOCK_WIDTH, (float)BLOCK_HEIGHT }) {}
	~Block();

	void OnBallHit(Ball& ball);
	void SetHealth(int value);

	void Update(float timeElapsed) override;

	const std::string textureKey;
	bool invulnerable = false;
	BallStatusEffect* appliesStatus;

private:
	int health = 1;
};

