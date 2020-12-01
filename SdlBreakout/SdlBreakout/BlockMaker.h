#pragma once
#include "stdafx.h"

class Block;

class BlockMaker
{
public:
	static Block* MakeNormal(int health);
	static Block* MakeInvulnerable();
};

