#pragma once

#include <string>
#include <map>
#include <SDL_mixer.h>

class Sounds
{
public:
	static const std::map<std::string, Mix_Chunk*>& GetTextures()
	{
		return sounds;
	}
	static Mix_Chunk* GetSound(const std::string& key)
	{
		return sounds[key];
	}
	static void LoadSounds();
	static void FreeAllSounds();

private:
	static std::map<std::string, Mix_Chunk*> sounds;
};

