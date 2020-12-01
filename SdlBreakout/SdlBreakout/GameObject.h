#pragma once

#include <SDL_image.h>
#include <memory>

#include "Vector2.h"
#include "Texture.h"
#include "Shape.h"

class GameObject
{
	// Memory management: GameObject supports copy construction and assignment. Subclasses may continue to support or restrict these operations.

public:
	GameObject() = default;
	explicit GameObject(const Shape* collisionBounds) : collisionBounds(collisionBounds->Clone()) { }
	GameObject(const GameObject& gameObject) : collisionBounds(gameObject.collisionBounds->Clone()), sprite(gameObject.sprite) { }
	virtual ~GameObject() {}

	GameObject& operator= (const GameObject& source)
	{
		sprite = source.sprite;
		collisionBounds = source.collisionBounds->Clone();
		return *this;
	}

	const Texture* getSprite();
	void setSprite(Texture* value);

	virtual void Update(float timeElapsed);

	std::unique_ptr<Shape> collisionBounds = nullptr;

protected:
	const Texture* sprite = nullptr;
};

