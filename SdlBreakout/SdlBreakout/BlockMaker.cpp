#include "stdafx.h"
#include "BlockMaker.h"

#include "BallStatus_Downfall.h"
#include "BallStatus_Accelerate.h"

Block* BlockMaker::MakeNormal(int health)
{
    auto block = new Block("normal");
    block->SetHealth(health);
    //TEMP
    block->appliesStatus = new BallStatus_Accelerate();
    //TEMP
    return block;
}

Block* BlockMaker::MakeInvulnerable()
{
    auto block = new Block("invulnerable");
    block->invulnerable = true;
    return block;
}
