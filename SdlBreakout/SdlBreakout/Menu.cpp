#include "stdafx.h"
#include "Menu.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "Fonts.h"
#include "RenderUtils.h"

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
			selectedIndex = (int)options.size() - 1;
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
	for (unsigned int i = 0; i < options.size(); i++)
	{
		auto& option = options[i];
		auto color = i == selectedIndex ? SDL_Color{ 255, 0, 0 } : SDL_Color{ 255, 255, 255 };

		RenderUtils::RenderText(Game::GetInstance().renderer, Vector2<int>(Game::SCREEN_WIDTH / 2, y), Vector2F(0.5, 0), option, Fonts::menuFont, color);
		y += 100;
	}

	return -1;
}
