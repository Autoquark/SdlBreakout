#include "stdafx.h"
#include "Input.h"

#include <SDL.h>

#include "Vector2.h"

Input::Input()
{
	keysDown = SDL_GetKeyboardState(&arraySize);
	keysPressed = std::vector<bool>(arraySize);
	keysReleased = std::vector<bool>(arraySize);
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

void Input::MouseEvent(SDL_MouseMotionEvent event)
{
	mouseMovement.x += event.xrel;
	mouseMovement.y += event.yrel;
}

void Input::EndUpdate()
{
	for (int i = 0; i < arraySize; i++)
	{
		keysPressed[i] = false;
		keysReleased[i] = false;
	}

	mouseMovement = Vector2F();
}
