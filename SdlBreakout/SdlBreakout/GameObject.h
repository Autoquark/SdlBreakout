#pragma once
#include <SDL_image.h>
#include "Vector2.h"
#include "Texture.h"

class GameObject
{
public:
	Vector2 position;

	Texture* getSprite();
	void setSprite(Texture* value);

	virtual void Update(float timeElapsed);

	GameObject(Vector2 size);
	~GameObject();

protected:
	Vector2 size;
	Texture* sprite;
};

