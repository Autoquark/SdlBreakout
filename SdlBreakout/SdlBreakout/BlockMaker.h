#pragma once
#include "stdafx.h"
#include "Block.h"

class BlockMaker
{
public:
	static std::unique_ptr<Block> MakeNormal(int health);
	static std::unique_ptr<Block> MakeInvulnerable();
};

