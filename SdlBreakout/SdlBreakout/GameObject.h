#pragma once
#include <SDL_image.h>
#include "Vector2.h"

class GameObject
{
public:
	Vector2 position;

	SDL_Texture* getSprite();
	void setSprite(SDL_Texture* value);

	virtual void Update(float timeElapsed);

	GameObject();
	~GameObject();

protected:
	Vector2 size;
	SDL_Texture* sprite;
};

