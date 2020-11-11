#include "stdafx.h"
#include "Sounds.h"

void Sounds::LoadSounds()
{
	sounds["bounce"] = Mix_LoadWAV("Sounds\\bounce.wav");
	if (sounds["bounce"] == NULL)
	{
		printf("Failed to load sound!\n");
		throw new std::exception();
	}

	sounds["bounce_invulnerable"] = Mix_LoadWAV("Sounds\\dullTwang.wav");
	if (sounds["bounce_invulnerable"] == NULL)
	{
		printf("Failed to load sound!\n");
		throw new std::exception();
	}
}

void Sounds::FreeAllSounds()
{
	for (const auto& [key, value] : sounds)
	{
		Mix_FreeChunk(value);
	}

	sounds.clear();
}

std::map<std::string, Mix_Chunk*> Sounds::sounds;