#pragma once
#include "stdafx.h"

#include "Block.h"

class BlockMaker
{
public:
	static Block* MakeNormal(int health);
	static Block* MakeInvulnerable();
};

