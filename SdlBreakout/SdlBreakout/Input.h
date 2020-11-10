#pragma once
#include "stdafx.h"

#include <SDL.h>
#include <SDL_scancode.h>

class Input
{
public:
	Input();

	[[nodiscard]]
	bool KeyPressed(SDL_Scancode key) const;
	[[nodiscard]]
	bool KeyReleased(SDL_Scancode key) const;
	[[nodiscard]]
	bool KeyIsDown(SDL_Scancode key) const;

	void KeyEvent(SDL_KeyboardEvent event);

	void EndUpdate();
private:
	int arraySize;
	// Pointer to the internal SDL keyboard state array which records whether keys are currently down
	const Uint8* keysDown;
	bool* keysPressed;
	bool* keysReleased;
};

