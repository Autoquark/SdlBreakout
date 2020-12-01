#include "stdafx.h"
#include "Input.h"

#include <SDL.h>

#include "Vector2.h"

Input::Input()
{
	keysDown = SDL_GetKeyboardState(&arraySize);
	keysPressed = std::vector<bool>(arraySize);
	keysReleased = std::vector<bool>(arraySize);

	mouseDown = std::vector<bool>(SDL_BUTTON_X2);
	mousePressed = std::vector<bool>(SDL_BUTTON_X2);
	mouseReleased = std::vector<bool>(SDL_BUTTON_X2);
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
	if (event.which == SDL_TOUCH_MOUSEID)
	{
		return;
	}

	mouseMovement.x += event.xrel;
	mouseMovement.y += event.yrel;

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mousePressed[event.which] = true;
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouseReleased[event.which] = true;
	}

	mouseDown[event.which] = event.state;
}

void Input::EndUpdate()
{
	for (int i = 0; i < arraySize; i++)
	{
		keysPressed[i] = keysReleased[i] = false;
	}

	for (int i = (int)MouseButton::Min; i < (int)MouseButton::Max; i++)
	{
		mousePressed[i] = mouseReleased[i] = false;
	}

	mouseMovement = Vector2F();
}
