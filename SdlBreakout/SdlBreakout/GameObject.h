#pragma once
#include <SDL_image.h>
#include "Vector2.h"
#include "Texture.h"
#include "AxisAlignedRectF.h"

class GameObject
{
public:
	Texture* getSprite();
	void setSprite(Texture* value);

	void SetPosition(Vector2F position)
	{
		collisionBounds.SetPosition(position);
	}
	void SetPosition(float x, float y)
	{
		collisionBounds.SetPosition(x, y);
	}
	const AxisAlignedRectF& GetCollisionBounds() const
	{
		return collisionBounds;
	}

	virtual void Update(float timeElapsed);

	GameObject(AxisAlignedRectF collisionBounds);
	~GameObject();

protected:
	AxisAlignedRectF collisionBounds;
	Texture* sprite = NULL;
};

