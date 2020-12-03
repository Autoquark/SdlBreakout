#include "stdafx.h"
#include "BlockMaker.h"

#include "Block.h"

std::unique_ptr<Block> BlockMaker::MakeNormal(int health)
{
    auto block = std::make_unique<Block>("normal");
    block->SetHealth(health);
    return block;
}

std::unique_ptr<Block> BlockMaker::MakeInvulnerable()
{
    auto block = std::make_unique<Block>("invulnerable");
    block->invulnerable = true;
    return block;
}
