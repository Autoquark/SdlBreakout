#pragma once
#include "stdafx.h"

#include "Texture.h"
#include "Textures.h"

class Powerup
{
public:
	virtual ~Powerup() {}

	virtual const Texture* GetIcon() const
	{
		return Textures::GetTexture("missingPowerup");
	}

	virtual void Activate() = 0;
};

