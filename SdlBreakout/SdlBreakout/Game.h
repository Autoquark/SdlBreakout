#pragma once
#include <SDL.h>
#include <vector>
#include "GameObject.h"
#include "Texture.h"
#include "AxisAlignedRectF.h"
#include "Paddle.h"
#include "block.h"

class Game
{
public:
	void operator=(Game const&) = delete;
	Game(Game const&) = delete;

	static Game& GetInstance();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const AxisAlignedRectF screenRect = AxisAlignedRectF(0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	const int TARGET_FPS = 60;

	const std::vector<Block*>& GetBlocks() const
	{
		return blocks;
	}

	void Destroy(GameObject* gameObject);

	// Sprites
	Texture* gBlockTexture = NULL;
	Texture* gBallTexture = NULL;
	Texture* gPaddleTexture = NULL;

	// The window renderer
	SDL_Renderer* gRenderer = NULL;

	// Special game objects
	Paddle* paddle = NULL;
	Ball* ball = NULL;

	int Start();

private:

	Game() = default;
	~Game();

	std::vector<GameObject*> gameObjects;
	std::vector<Block*> blocks;

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The surface contained by the window
	SDL_Surface* gScreenSurface = NULL;

	//Current displayed texture
	SDL_Texture* gTexture = NULL;

	bool init();
	bool loadMedia();
	Texture* loadTexture(std::string path);
	SDL_Surface* loadSurface(std::string path);
	void close();
};
