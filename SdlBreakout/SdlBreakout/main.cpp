#include "stdafx.h"
#include <stdio.h>
#include <optional>
#include "game.h"

int main(int argc, char* args[])
{
	Game::GetInstance().Start();

	return 0;
}