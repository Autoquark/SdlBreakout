#include "stdafx.h"

#include <filesystem>

#include "Textures.h"
#include "Game.h"

bool Textures::LoadTextures()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	textures["bounds"] = LoadTexture("Images\\bounds.png");
	textures["block"] = LoadTexture("Images\\block.png");
	textures["ball"] = LoadTexture("Images\\ball.png");
	textures["paddle"] = LoadTexture("Images\\paddle_rounded.png");

	textures["missingStatus"] = LoadTexture("Images\\missingStatus.png");
	textures["downfall"] = LoadTexture("Images\\downfall.png");
	textures["accelerate"] = LoadTexture("Images\\accelerate.png");

	blockTextures["normal"] = LoadTextureSequence("Images\\block");
	blockTextures["invulnerable"] = LoadTextureSequence("Images\\invulnerable");

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
		throw new std::exception();
	}

	//Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance().renderer, loadedSurface);

	SDL_FreeSurface(loadedSurface);

	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		throw new std::exception();
	}

	return new Texture(newTexture, Vector2<int>{ loadedSurface->w, loadedSurface->h });
}

std::vector<Texture*> Textures::LoadTextureSequence(std::string pathPrefix)
{
	std::vector<Texture*> textures;
	for(int i = 1; true; i++)
	{
		auto filename = pathPrefix + std::to_string(i) + ".png";
		if (!std::filesystem::exists(filename))
		{
			break;
		}
		textures.push_back(LoadTexture(filename));
	}

	return textures;
}

std::map<std::string, Texture*> Textures::textures;
std::map<std::string, std::vector<Texture*>> Textures::blockTextures;
