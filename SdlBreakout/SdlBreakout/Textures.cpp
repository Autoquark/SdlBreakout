#include "stdafx.h"
#include "Textures.h"
#include "Game.h"

bool Textures::LoadTextures()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	textures["bounds"] = LoadTexture("Images\\bounds.png");
	if (textures["bounds"] == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	textures["block"] = LoadTexture("Images\\block.png");
	if (textures["block"] == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	textures["ball"] = LoadTexture("Images\\ball.png");
	if (textures["ball"] == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	textures["paddle"] = LoadTexture("Images\\paddle_rounded.png");
	if (textures["paddle"] == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	return success;
}

void Textures::FreeAllTextures()
{
	for(const auto& [key, value] : textures)
	{
		SDL_DestroyTexture(value->GetSdlTexture());
	}

	textures.clear();
}

Texture* Textures::LoadTexture(std::string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance().renderer, loadedSurface);

	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		SDL_FreeSurface(loadedSurface);
		return NULL;
	}

	SDL_FreeSurface(loadedSurface);

	auto texture = new Texture(newTexture, Vector2<int>{ loadedSurface->w, loadedSurface->h });

	return texture;
}

std::map<std::string, Texture*> Textures::textures;
