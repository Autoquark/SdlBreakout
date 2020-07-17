#include "GameObject.h"
#include "Ball.h"

#pragma once
class Block : public GameObject
{
public:
	Block();
	~Block();

	void OnBallHit(Ball& ball);
};

