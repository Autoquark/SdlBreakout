#pragma once
#include "stdafx.h"

#include <map>

#include "Texture.h"

class Textures
{
public:
	static const std::map<std::string, Texture*>& GetTextures()
	{
		return textures;
	}
	static Texture* GetTexture(const std::string& key)
	{
		return textures[key];
	}
	static bool LoadTextures();
	static void FreeAllTextures();

private:
	static std::map<std::string, Texture*> textures;

	static Texture* LoadTexture(std::string path);
};

