#include "stdafx.h"
#include <algorithm>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Game.h"
#include "Paddle.h"
#include "ball.h"
#include "block.h"
#include "GameObject.h"
#include "Texture.h"
#include "Shape.h"

Game::~Game()
{
}

Game& Game::GetInstance()
{
	static Game game;
	return game;
}

void Game::Destroy(GameObject * gameObject)
{
	blocks.erase(std::find(blocks.begin(), blocks.end(), gameObject));
	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
	delete gameObject;
}

int Game::Start()
{
	if (!init())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	if (!loadMedia())
	{
		printf("Failed to load media!\n");
		return -1;
	}

	for (int y = 120; y < 360; y += (int)gBlockTexture->GetSize().y)
	{
		for (int x = 240; x < 480; x += (int)gBlockTexture->GetSize().x)
		{
			blocks.push_back(new Block());
			blocks.back()->collisionBounds->Translate((float)x, (float)y);
		}
	}

	for (auto block : blocks)
	{
		gameObjects.push_back(block);
	}

	paddle = new Paddle();
	gameObjects.push_back(paddle);
	gameObjects.back()->collisionBounds->Translate(320, 440);

	gameObjects.push_back(new Ball());
	gameObjects.back()->collisionBounds->Translate(320, 400);

	bool quit = false;

	auto lastUpdateTime = SDL_GetTicks();

	while (!quit)
	{
		//Event handler
		SDL_Event e;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(gRenderer);

		auto time = SDL_GetTicks();
		// If the update loop goes over our time budget (perhaps due to debugging), don't let the elapsed time grow too big
		float elapsed = (float)std::min((time - lastUpdateTime) / 1000.0, 1.0 / TARGET_FPS);

		for (auto gameObject : gameObjects)
		{
			gameObject->Update(elapsed);
		}

		lastUpdateTime = time;

		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	gBlockTexture = loadTexture("Images\\block.png");
	if (gBlockTexture == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	gBallTexture = loadTexture("Images\\squareBall.png");
	if (gBallTexture == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	gPaddleTexture = loadTexture("Images\\paddle.png");
	if (gBallTexture == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	return success;
}

bool Game::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

Texture* Game::loadTexture(std::string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);	

	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		SDL_FreeSurface(loadedSurface);
		return NULL;
	}

	SDL_FreeSurface(loadedSurface);

	auto texture = new Texture(newTexture, Vector2<int>{ loadedSurface->w, loadedSurface->h });

	return texture;
}

SDL_Surface* Game::loadSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	//Convert surface to screen format
	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	return optimizedSurface;
}

void Game::close()
{
	//Free loaded image
	SDL_DestroyTexture(gBlockTexture->GetSdlTexture());
	SDL_DestroyTexture(gBallTexture->GetSdlTexture());

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}