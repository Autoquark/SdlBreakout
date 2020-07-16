#include <SDL_image.h>
#include "Vector2.h"

#pragma once
class Texture
{
public:
	Texture(SDL_Texture*& sdlTexture, const Vector2<int>& size);
	SDL_Texture* GetSdlTexture();
	Vector2<int> GetSize();

private:
	SDL_Texture* sdlTexture;
	const Vector2<int> size;
};

