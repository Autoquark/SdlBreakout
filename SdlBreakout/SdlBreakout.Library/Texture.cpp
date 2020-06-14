#include "stdafx.h"
#include "Texture.h"

Texture::Texture(SDL_Texture*& sdlTexture, const Vector2& size) : sdlTexture(sdlTexture), size(size)
{
}

SDL_Texture* Texture::GetSdlTexture()
{
	return sdlTexture;
}

Vector2 Texture::GetSize()
{
	return size;
}
