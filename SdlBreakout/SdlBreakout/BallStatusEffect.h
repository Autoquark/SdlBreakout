#pragma once
#include "stdafx.h"

#include "Ball.h"

class BallStatusEffect
{
public:
	BallStatusEffect(Ball& ball) : ball(ball)
	{
	}

	virtual void Update(float timeElapsed) {}

	virtual void OnRemoved(){}

private:
	Ball& ball;
};

