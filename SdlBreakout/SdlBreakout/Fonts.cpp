#include "stdafx.h"
#include "Fonts.h"

void Fonts::LoadFonts()
{
	//menuFont = TTF_OpenFont("Fonts/MB-Demonic_Tale.ttf", 36);
	menuFont = TTF_OpenFont("Fonts/Fiolex_Mephisto.ttf", 36);
}

TTF_Font* Fonts::menuFont;
