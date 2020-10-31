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

	const std::string textureKey;
	bool invulnerable = false;

private:
	int health = 1;
};

