#pragma once
#include <SDL_image.h>
#include "Vector2.h"
#include "Texture.h"
#include "RectF.h"

class GameObject
{
public:
	Texture* getSprite();
	void setSprite(Texture* value);

	void SetPosition(Vector2 position)
	{
		collisionBounds.SetPosition(position);
	}
	void SetPosition(float x, float y)
	{
		collisionBounds.SetPosition(x, y);
	}
	const RectF& GetCollisionBounds() const
	{
		return collisionBounds;
	}

	virtual void Update(float timeElapsed);

	GameObject(RectF collisionBounds);
	~GameObject();

protected:
	RectF collisionBounds;
	Texture* sprite;
};

