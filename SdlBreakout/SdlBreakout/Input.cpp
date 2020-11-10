#include "stdafx.h"
#include "Input.h"

#include <SDL.h>

Input::Input()
{
	keysDown = SDL_GetKeyboardState(&arraySize);
	keysPressed = new bool[arraySize];
	keysReleased = new bool[arraySize];
}

bool Input::KeyPressed(SDL_Scancode key) const
{
	return keysPressed[key];
}

bool Input::KeyReleased(SDL_Scancode key) const
{
	return keysReleased[key];
}

bool Input::KeyIsDown(SDL_Scancode key) const
{
	return keysDown[key];
}

void Input::KeyEvent(SDL_KeyboardEvent event)
{
	if (event.type == SDL_KEYDOWN && event.repeat == 0)
	{
		keysPressed[event.keysym.scancode] = true;
	}
	else if (event.type == SDL_KEYUP)
	{
		keysReleased[event.keysym.scancode] = true;
	}
}

void Input::EndUpdate()
{
	for (int i = 0; i < arraySize; i++)
	{
		keysPressed[i] = false;
		keysReleased[i] = false;
	}
}
