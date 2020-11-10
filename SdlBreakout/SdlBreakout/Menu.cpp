#include "stdafx.h"
#include "Menu.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"

int Menu::Update()
{
	if (Game::GetInput().KeyPressed(SDL_SCANCODE_RETURN))
	{
		return selectedIndex;
	}

	if (Game::GetInput().KeyPressed(SDL_SCANCODE_UP))
	{
		selectedIndex--;
		if (selectedIndex == -1)
		{
			selectedIndex = options.size() - 1;
		}
	}
	if (Game::GetInput().KeyPressed(SDL_SCANCODE_DOWN))
	{
		selectedIndex++;
		if (selectedIndex == options.size())
		{
			selectedIndex = 0;
		}
	}

	int y = 100;
	for (int i = 0; i < options.size(); i++)
	{
		auto option = (options)[i];
		auto font = TTF_OpenFont("Fonts/MB-Demonic_Tale.ttf", 24);
		auto error = TTF_GetError();
		auto color = i == selectedIndex ? SDL_Color{ 255, 0, 0 } : SDL_Color{ 255, 255, 255 };
		auto surfaceMessage = TTF_RenderText_Solid(font, option.c_str(), color);
		auto renderer = Game::GetInstance().renderer;
		auto message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		int width, height;
		TTF_SizeText(font, option.c_str(), &width, &height);

		auto rect = SDL_Rect{ (Game::SCREEN_WIDTH - width) / 2, y, width, height };
		SDL_RenderCopy(renderer, message, NULL, &rect);
		y += 100;
	}

	return -1;
}
