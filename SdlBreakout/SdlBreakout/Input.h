#pragma once
#include "stdafx.h"

#include <SDL.h>
#include <SDL_scancode.h>
#include <vector>

#include "Vector2.h"

class Input
{
public:
	enum class MouseButton
	{
		Left = SDL_BUTTON_LEFT,
		Middle = SDL_BUTTON_MIDDLE,
		Right = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2,

		Min = Left,
		Max = X2
	};

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

	[[nodiscard]]
	bool MouseButtonPressed(MouseButton button) const
	{
		assert(MouseButton::Min <= button && button <= MouseButton::Max);
		// cppcheck-suppress negativeContainerIndex
		return mousePressed[(int)button];
	}

	[[nodiscard]]
	bool MouseButtonReleased(MouseButton button) const
	{
		assert(MouseButton::Min <= button && button <= MouseButton::Max);
		// cppcheck-suppress negativeContainerIndex
		return mouseReleased[(int)button];
	}

	[[nodiscard]]
	bool MouseButtonDown(MouseButton button) const
	{
		assert(MouseButton::Min <= button && button <= MouseButton::Max);
		// cppcheck-suppress negativeContainerIndex
		return mouseDown[(int)button];
	}

	// Returns a vector representing the movement of the mouse since the last frame
	[[nodiscard]]
	Vector2F GetMouseMovement() const
	{
		return mouseMovement;
	}

	void KeyEvent(SDL_KeyboardEvent event);

	void MouseMotionEvent(SDL_MouseMotionEvent event);

	void MouseButtonEvent(SDL_MouseButtonEvent);

	void EndUpdate();
private:
	int arraySize;
	// Pointer to the internal SDL keyboard state array which records whether keys are currently down
	const Uint8* keysDown;
	std::vector<bool> keysPressed;
	std::vector<bool> keysReleased;

	std::vector<bool> mouseDown;
	std::vector<bool> mousePressed;
	std::vector<bool> mouseReleased;

	Vector2F mouseMovement = Vector2F();

};

