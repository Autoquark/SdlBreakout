#include "ball.h"
#include "main.h"


Ball::Ball()
{
	sprite = gBallTexture;
	velocity.x = -10;
	velocity.y = -30;
}


Ball::~Ball()
{
}

void Ball::Update(float timeElapsed)
{
	auto remainingVelocity = velocity * timeElapsed;
	while (true)
	{
		auto nextPosition = position + remainingVelocity;
		float bestProportion = 2;

		int best = 0;
		if (nextPosition.x < 0)
		{
			bestProportion = position.x / -remainingVelocity.x;
			best = 1;
		}
		if (nextPosition.x > SCREEN_WIDTH)
		{
			auto proportion = (SCREEN_WIDTH - position.x) / remainingVelocity.x;
			if (proportion < bestProportion)
			{
				bestProportion = proportion;
				best = 2;
			}
		}
		if (nextPosition.y < 0)
		{
			auto proportion = position.y / -remainingVelocity.y;
			if (proportion < bestProportion)
			{
				bestProportion = proportion;
				best = 3;
			}
		}
		if (nextPosition.y > SCREEN_HEIGHT)
		{
			auto proportion = (SCREEN_HEIGHT - position.y) / remainingVelocity.y;
			if (proportion < bestProportion)
			{
				bestProportion = proportion;
				best = 4;
			}
		}

		if (best == 0)
		{
			position = nextPosition;
			break;
		}

		position += bestProportion * remainingVelocity;

		switch (best)
		{
		case 1:
		case 2:
			velocity.x = -velocity.x;
			break;

		case 3:
		case 4:
			velocity.y = -velocity.y;

		default:
			break;
		}

		remainingVelocity = (1 - bestProportion) * velocity * timeElapsed;
	}

	GameObject::Update(timeElapsed);
}