#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "AxisAlignedRectF.h"

class Ball;
class BallStatusEffect;

class Block : public GameObject
{
public:
	static const int BLOCK_WIDTH = 40;
	static const int BLOCK_HEIGHT = 20;

	Block(const std::string& textureKey, const Shape& collisionBounds);
	explicit Block(const std::string& textureKey) : Block(textureKey, *new AxisAlignedRectF{ 0, 0, (float)BLOCK_WIDTH, (float)BLOCK_HEIGHT }) {}
	~Block();

	void OnBallHit(Ball& ball);
	void SetHealth(int value);

	void Update(float timeElapsed) override;

	std::string textureKey;
	bool invulnerable = false;

	// Status effect that this block applies to the ball upon being hit.
	// Multiple blocks may reference the same BallStatusEffect
	const BallStatusEffect* appliesStatus = nullptr;

private:
	static const float POWERUP_DROP_CHANCE;

	int health = 1;
};

