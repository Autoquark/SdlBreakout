#pragma once
#include "stdafx.h"

#include <SDL.h>
#include <SDL_scancode.h>
#include <vector>

#include "Vector2.h"

class Input
{
public:
	Input();
	Input(const Input&) = delete;

	Input& operator= (const Input& source) = delete;

	[[nodiscard]]
	bool KeyPressed(SDL_Scancode key) const
	{
		return keysPressed[key];
	}

	[[nodiscard]]
	bool KeyReleased(SDL_Scancode key) const
	{
		return keysReleased[key];
	}

	[[nodiscard]]
	bool KeyIsDown(SDL_Scancode key) const
	{
		return keysDown[key];
	}

	// Returns a vector representing the movement of the mouse since the last frame
	[[nodiscard]]
	Vector2F GetMouseMovement() const
	{
		return mouseMovement;
	}

	void KeyEvent(SDL_KeyboardEvent event);

	void MouseEvent(SDL_MouseMotionEvent event);

	void EndUpdate();
private:
	int arraySize;
	// Pointer to the internal SDL keyboard state array which records whether keys are currently down
	const Uint8* keysDown;
	std::vector<bool> keysPressed;
	std::vector<bool> keysReleased;
	Vector2F mouseMovement = Vector2F();
};

