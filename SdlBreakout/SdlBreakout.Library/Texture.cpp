#include "stdafx.h"
#include "Texture.h"

Texture::Texture(SDL_Texture*& sdlTexture, const Vector2<int>& size) : sdlTexture(sdlTexture), size(size)
{
}

SDL_Texture* Texture::GetSdlTexture() const
{
	return sdlTexture;
}

Vector2<int> Texture::GetSize() const
{
	return size;
}
