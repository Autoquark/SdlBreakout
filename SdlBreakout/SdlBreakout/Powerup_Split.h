#pragma once
#include "stdafx.h"
#include "Powerup.h"

class Powerup_Split : public Powerup
{
	virtual const Texture* GetIcon() const override
	{
		return Textures::GetTexture("powerup_split2");
	}

	virtual void Activate() override;
};

