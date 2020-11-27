#pragma once

#include <SDL_image.h>
#include "Vector2.h"
#include "Texture.h"
#include "AxisAlignedRectF.h"

class GameObject
{
public:
	GameObject(const GameObject& gameObject) : collisionBounds(gameObject.collisionBounds->Clone())
	{
	}
	virtual ~GameObject() {}

	const Texture* getSprite();
	void setSprite(Texture* value);

	virtual void Update(float timeElapsed);

	const std::unique_ptr<Shape> collisionBounds;

	GameObject(Shape* collisionBounds = NULL) : collisionBounds(collisionBounds->Clone())
	{

	}

protected:
	const Texture* sprite = NULL;
};

