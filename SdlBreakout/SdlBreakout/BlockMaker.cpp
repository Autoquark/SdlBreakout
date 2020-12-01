#include "stdafx.h"
#include "BlockMaker.h"

#include "Block.h"

Block* BlockMaker::MakeNormal(int health)
{
    auto block = new Block("normal");
    block->SetHealth(health);
    return block;
}

Block* BlockMaker::MakeInvulnerable()
{
    auto block = new Block("invulnerable");
    block->invulnerable = true;
    return block;
}
