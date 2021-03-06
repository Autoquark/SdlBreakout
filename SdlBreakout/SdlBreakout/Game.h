#pragma once
#include <SDL.h>
#include <vector>
#include <random>

#include "Input.h"
#include "AxisAlignedRectF.h"
#include "Level.h"

class Menu;

class Game
{
public:
	void operator=(Game const&) = delete;
	Game(Game const&) = delete;

	static Game& GetInstance();

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;
	static const bool DISABLE_BALL_LOSS = false;
	static const bool PAUSE_ON_FOCUS_LOSS = false;
	static const AxisAlignedRectF screenRect;
	static const AxisAlignedRectF levelArea;

	const int TARGET_FPS = 60;

	static const Input& GetInput()
	{
		return GetInstance().input;
	}

	static Level* GetLevel()
	{
		return GetInstance().level.get();
	}

	// Gets the current game time in seconds. This excludes time spent paused.
	float GetTime()
	{
		return time;
	}

	// The window renderer
	SDL_Renderer* renderer = NULL;

	bool drawCollisionShapes = false;
	int lives = 3;
	int score = 0;
	std::default_random_engine random;

	void Start();

private:

	static const float SCORE_UPDATE_TIME;

	Game();
	~Game();

	std::unique_ptr<Menu> currentMenu = nullptr;

	//The window we'll be rendering to
	SDL_Window* window = NULL;
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	Input input;
	std::unique_ptr<Level> level;

	float time = 0;
	float displayScore = 0;
	float scoreIncreaseRate = 0;

	void init();
	void RenderUi(float timeElapsed);
	SDL_Surface* loadSurface(std::string path);
};