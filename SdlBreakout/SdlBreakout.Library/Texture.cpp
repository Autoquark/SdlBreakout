#include "stdafx.h"
#include "Texture.h"

Texture::Texture(SdlTextureUniquePtr& sdlTexture, const Vector2<int>& size) : sdlTexture(std::move(sdlTexture)), size(size)
{
}

SDL_Texture* Texture::GetSdlTexture() const
{
	return sdlTexture.get();
}
