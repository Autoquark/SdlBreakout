#pragma once
#include "GameObject.h"

class Paddle : public GameObject
{
public:
	Paddle();
	~Paddle();

	void Update(float timeElapsed) override;

private:
	float moveSpeed;
};

