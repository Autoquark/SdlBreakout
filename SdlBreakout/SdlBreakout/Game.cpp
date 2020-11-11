#include "stdafx.h"

#include <algorithm>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Input.h"
#include "Game.h"
#include "Paddle.h"
#include "Ball.h"
#include "BlockMaker.h"
#include "GameObject.h"
#include "Texture.h"
#include "Shape.h"
#include "Bounds.h"
#include "Textures.h"
#include "Sounds.h"
#include "Fonts.h"

Game::~Game()
{
	//Free loaded image
	Textures::FreeAllTextures();
	Sounds::FreeAllSounds();

	//Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
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

void Game::Start()
{
	init();

	Textures::LoadTextures();
	Sounds::LoadSounds();
	Fonts::LoadFonts();

	int x,y;
	for (y = 120; y < 340; y += (int)Textures::GetTexture("block")->GetSize().y)
	{
		for (x = 240; x < 480; x += (int)Textures::GetTexture("block")->GetSize().x)
		{
			blocks.push_back(BlockMaker::MakeNormal(3));
			blocks.back()->collisionBounds->Translate((float)x, (float)y);
		}
	}

	for (x = 240; x < 480; x += (int)Textures::GetTexture("block")->GetSize().x)
	{
		blocks.push_back(BlockMaker::MakeInvulnerable());
		blocks.back()->collisionBounds->Translate((float)x, (float)y);
	}

	for (auto block : blocks)
	{
		gameObjects.push_back(block);
	}

	paddle = new Paddle();
	gameObjects.push_back(paddle);
	gameObjects.back()->collisionBounds->Translate(320, 440);

	ball = new Ball();
	gameObjects.push_back(ball);
	gameObjects.back()->collisionBounds->Translate(320, 400);

	bounds = new Bounds();
	gameObjects.push_back(bounds);

	bool quit = false;

	auto lastUpdateSdlTime = SDL_GetTicks();

	while (!quit)
	{
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
			if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
			{
				input.KeyEvent(e.key);
			}
		}

		if (!currentMenu && input.KeyPressed(SDL_SCANCODE_ESCAPE))
		{
			currentMenu = new Menu();
			currentMenu->Show(std::vector{ "Resume"s, "Quit"s });
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		if (currentMenu)
		{
			auto value = currentMenu->Update();
			if (value == 0)
			{
				delete currentMenu;
				currentMenu = nullptr;
				// Ignore any time passed while the menu was open
				lastUpdateSdlTime = SDL_GetTicks();
			}
			else if (value == 1)
			{
				break;
			}
		}
		else
		{

			auto sdlTime = SDL_GetTicks();
			// If the update loop goes over our time budget (perhaps due to debugging), don't let the elapsed time grow too big
			float elapsed = (float)std::min((sdlTime - lastUpdateSdlTime) / 1000.0, 1.0 / TARGET_FPS);
			time += elapsed;

			for (auto gameObject : gameObjects)
			{
				gameObject->Update(elapsed);
			}

			// Debug tool: Speed up when space held
			if (input.KeyIsDown(SDL_SCANCODE_SPACE))
			{
				for (auto gameObject : gameObjects)
				{
					gameObject->Update(elapsed);
				}
			}

			lastUpdateSdlTime = sdlTime;
		}

		input.EndUpdate();
		SDL_RenderPresent(renderer);
	}
}

void Game::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		throw new std::exception();
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		throw new std::exception();
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		throw new std::exception();
	}

	// Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		throw new std::exception();
	}

	// Initialize SDL_TTF
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		throw new std::exception();
	}

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		throw new std::exception();
	}
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