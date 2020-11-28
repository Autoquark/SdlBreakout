#include <SDL_image.h>
#include <SDL_render.h>
#include <memory>
#include "Vector2.h"
#include "SDL_Deleter.h"

#pragma once
class Texture
{
public:
	Texture(SdlTextureUniquePtr& sdlTexture, const Vector2<int>& size);

	SDL_Texture* GetSdlTexture() const;
	const Vector2<int>& GetSize() const;

private:
	const SdlTextureUniquePtr sdlTexture;
	const Vector2<int> size;
};

