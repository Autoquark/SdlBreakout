#pragma once
#include "stdafx.h"

#include <map>

#include "Texture.h"

class Textures
{
public:
	static Texture* GetTexture(const std::string& key)
	{
		return textures[key];
	}
	static const std::vector<Texture*>& GetBlockTextures(const std::string& key)
	{
		return blockTextures[key];
	}
	static bool LoadTextures();
	static void FreeAllTextures();

private:
	static std::map<std::string, Texture*> textures;
	static std::map<std::string, std::vector<Texture*>> blockTextures;

	static Texture* LoadTexture(std::string path);
	static std::vector<Texture*> LoadTextureSequence(std::string pathPrefix);
};

