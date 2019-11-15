#include "ball.h"
#include "main.h"


Ball::Ball()
{
	sprite = gBallTexture;
	velocity.y = -100;
}


Ball::~Ball()
{
}

void Ball::Update(float timeElapsed)
{
	position += velocity * timeElapsed;

	GameObject::Update(timeElapsed);
}