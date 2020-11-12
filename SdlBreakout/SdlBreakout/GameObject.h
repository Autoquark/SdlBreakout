#pragma once

#include <SDL_image.h>
#include "Vector2.h"
#include "Texture.h"
#include "AxisAlignedRectF.h"

class GameObject
{
public:
	virtual ~GameObject()
	{
		delete collisionBounds;
	}

	const Texture* getSprite();
	void setSprite(Texture* value);

	virtual void Update(float timeElapsed);

	Shape* const collisionBounds;

	GameObject(Shape* collisionBounds = NULL) : collisionBounds(collisionBounds) {}

protected:
	const Texture* sprite = NULL;
};

