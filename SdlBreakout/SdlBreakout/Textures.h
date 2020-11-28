#pragma once
#include "stdafx.h"

#include <map>
#include <vector>
#include <filesystem>

#include "Texture.h"

class Textures
{
public:
	static const Texture* GetTexture(const std::string& key)
	{
		return textures.at(key).get();
	}
	static std::vector<Texture*> GetBlockTextures(const std::string& key)
	{
		std::vector<Texture*> results;
		const auto& vector = blockTextures.at(key);
		std::transform(vector.cbegin(), vector.cend(), std::back_inserter(results), [](const std::unique_ptr<Texture>& unique) { return unique.get(); });
		return results;
	}
	static void LoadTextures();
	static void FreeAllTextures();

private:
	static std::map<std::string, std::unique_ptr<Texture>> textures;
	static std::map<std::string, std::vector<std::unique_ptr<Texture>>> blockTextures;

	static std::unique_ptr<Texture> LoadTexture(const std::filesystem::path path);
	static std::vector<std::unique_ptr<Texture>> LoadTextureSequence(const std::filesystem::path directory);
};

