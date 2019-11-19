#pragma once
#include <SDL.h>
#include <vector>
#include "GameObject.h"

class Game
{
public:
	static Game* GetInstance();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const int TARGET_FPS = 60;

	// Sprites
	SDL_Texture* gBlockTexture = NULL;
	SDL_Texture* gBallTexture = NULL;
	SDL_Texture* gPaddleTexture = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	int Start();

	Game();
	~Game();

private:

	std::vector<GameObject*> gameObjects;

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//Current displayed texture
	SDL_Texture* gTexture = NULL;

	bool init();
	bool loadMedia();
	SDL_Texture* loadTexture(std::string path);
	SDL_Surface* loadSurface(std::string path);
	void close();
};

