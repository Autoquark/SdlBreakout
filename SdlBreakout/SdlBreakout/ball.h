#pragma once
#include "GameObject.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	void Update(float timeElapsed) override;
	Vector2F velocity;
};

