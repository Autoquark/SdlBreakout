#include "GameObject.h"
#include "Ball.h"

#pragma once
class Block : public GameObject
{
public:
	Block(std::string textureKey);
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

