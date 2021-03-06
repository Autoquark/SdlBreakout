#include "stdafx.h"
#include "Game.h"

#include <algorithm>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>

#include "GameObject.h"
#include "Textures.h"
#include "Sounds.h"
#include "Fonts.h"
#include "Menu.h"
#include "RenderUtils.h"

Game::Game() : random((int)std::time(0))
{
}

Game::~Game()
{
	//Free loaded image
	Textures::FreeAllTextures();
	Sounds::FreeAllSounds();

	//Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Game& Game::GetInstance()
{
	static Game game;
	return game;
}

void Game::Start()
{
	init();

	Textures::LoadTextures();
	Sounds::LoadSounds();
	Fonts::LoadFonts();

	std::vector<std::string> levels =
	{
		"1.json",
		"rows.json",
		"stippled.json"
	};
	auto levelIndex = 0;
	level = Level::Load("Data/Levels/" + levels[levelIndex]);

	auto lastUpdateSdlTime = SDL_GetTicks();

	while (true)
	{
		SDL_Event e;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				break;
			}
			if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
			{
				input.KeyEvent(e.key);
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				input.MouseMotionEvent(e.motion);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				input.MouseButtonEvent(e.button);
			}
		}

		auto escapeHandled = false;
		if (!currentMenu
			&& (input.KeyPressed(SDL_SCANCODE_ESCAPE)
				|| (!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) && PAUSE_ON_FOCUS_LOSS)))
		{
			escapeHandled = true;
			currentMenu = std::make_unique<Menu>();
			currentMenu->Show(std::vector{ "Resume"s, "Quit"s });
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		if (currentMenu)
		{
			auto value = currentMenu->Update();
			if (value == 0 || (!escapeHandled && input.KeyPressed(SDL_SCANCODE_ESCAPE)))
			{
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

			RenderUi(elapsed);

			auto result = level->Update(elapsed);
			if (result == Level::UpdateResult::Defeat)
			{
				break;
			}
			else if (result == Level::UpdateResult::Victory)
			{
				lives++;
				levelIndex++;
				if (levelIndex == levels.size())
				{
					// Just loop back to the start for now
					levelIndex = 0;
				}
				level = Level::Load("Data/Levels/" + levels[levelIndex]);
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
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		throw new std::exception();
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	screenSurface = SDL_GetWindowSurface(window);

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		throw new std::exception();
	}

	if (SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE) != 0)
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
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}

	return optimizedSurface;
}

void Game::RenderUi(float timeElapsed)
{
	const auto* sprite = Textures::GetTexture("life");

	Vector2 position(0, 0);
	for (int i = 0; i < lives; i++)
	{
		RenderUtils::RenderTexture(renderer, position, Vector2F(), *sprite);

		position.x += (int)(sprite->size.x * 1.5);
	}

	auto margin = 2;
	scoreIncreaseRate = std::max(scoreIncreaseRate, (score - displayScore) / SCORE_UPDATE_TIME);
	displayScore += scoreIncreaseRate * timeElapsed;
	if (displayScore >= score)
	{
		displayScore = (float)score;
		scoreIncreaseRate = 0;
	}

	RenderUtils::RenderText(renderer, Vector2<int>(Game::SCREEN_WIDTH - margin, margin), Vector2F(1, 0), std::to_string((int)displayScore), Fonts::scoreFont, SDL_Color{ 255, 255, 255 }, true);
}

const float Game::SCORE_UPDATE_TIME = 0.5;

const AxisAlignedRectF Game::screenRect = AxisAlignedRectF(0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
const AxisAlignedRectF Game::levelArea = AxisAlignedRectF(0.0f, 20.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT - 20.0f);