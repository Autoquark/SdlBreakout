#pragma once
#include <memory>
#include <SDL_image.h>

// From https://swarminglogic.com/jotting/2015_05_smartwrappers

struct SDL_Deleter
{
	void operator()(SDL_Surface* ptr) { if (ptr) SDL_FreeSurface(ptr); }
	void operator()(SDL_Texture* ptr) { if (ptr) SDL_DestroyTexture(ptr); }
	void operator()(SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
	void operator()(SDL_Window* ptr) { if (ptr) SDL_DestroyWindow(ptr); }
	void operator()(SDL_RWops* ptr) { if (ptr) SDL_RWclose(ptr); }
};

// Unique Pointers
using SurfaceUniquePtr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
using SdlTextureUniquePtr = std::unique_ptr<SDL_Texture, SDL_Deleter>;
using RendererUniquePtr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using WindowUniquePtr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using RWopsUniquePtr = std::unique_ptr<SDL_RWops, SDL_Deleter>;
