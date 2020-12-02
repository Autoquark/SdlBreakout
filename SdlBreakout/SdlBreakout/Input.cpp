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

void Input::MouseMotionEvent(SDL_MouseMotionEvent event)
{
	mouseMovement.x += event.xrel;
	mouseMovement.y += event.yrel;
}

void Input::MouseButtonEvent(SDL_MouseButtonEvent event)
{
	if (event.button < (int)MouseButton::Min || event.which >(int)MouseButton::Max)
	{
		return;
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mousePressed[event.button] = true;
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		mouseReleased[event.button] = true;
	}

	mouseDown[event.button] = event.state;
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
