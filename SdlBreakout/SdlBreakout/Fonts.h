#pragma once
#include "SDL_ttf.h"

class Fonts
{
public:
	static TTF_Font* menuFont;

	static void LoadFonts();
};

