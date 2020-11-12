#include "stdafx.h"

#include <filesystem>
#include <vector>

#include "Textures.h"
#include "Game.h"

void Textures::LoadTextures()
{
	for (auto& file : std::filesystem::directory_iterator("Images"))
	{
		if (file.is_directory())
		{
			blockTextures[file.path().filename().string()] = LoadTextureSequence(file.path());
		}
		else
		{
			textures[file.path().filename().replace_extension().string()] = LoadTexture(file.path().string());
		}
	}
}

void Textures::FreeAllTextures()
{
	for(const auto& [key, value] : textures)
	{
		SDL_DestroyTexture(value->GetSdlTexture());
	}

	textures.clear();
}

Texture* Textures::LoadTexture(const std::filesystem::path path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.string().c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.string().c_str(), IMG_GetError());
		throw new std::exception();
	}

	//Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance().renderer, loadedSurface);

	SDL_FreeSurface(loadedSurface);

	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.string().c_str(), SDL_GetError());
		throw new std::exception();
	}

	return new Texture(newTexture, Vector2<int>{ loadedSurface->w, loadedSurface->h });
}

std::vector<Texture*> Textures::LoadTextureSequence(const std::filesystem::path directory)
{
	std::vector<std::filesystem::directory_entry> files;
	std::copy(std::filesystem::directory_iterator(directory), std::filesystem::directory_iterator(), std::back_inserter(files));
	std::sort(files.begin(), files.end());

	std::vector<Texture*> textures;
	for(auto file : files)
	{
		if (file.is_directory() || file.path().extension() != ".png")
		{
			continue;
		}
		textures.push_back(LoadTexture(file.path()));
	}

	return textures;
}

std::map<std::string, Texture*> Textures::textures;
std::map<std::string, std::vector<Texture*>> Textures::blockTextures;
