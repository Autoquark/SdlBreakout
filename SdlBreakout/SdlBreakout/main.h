#pragma once
#include <SDL.h>

//The surface contained by the window
extern SDL_Surface* gScreenSurface;

extern SDL_Renderer* gRenderer;

// Sprites
extern SDL_Texture* gBlockTexture;
extern SDL_Texture* gBallTexture;
extern SDL_Texture* gPaddleTexture;