#pragma once
#include "stdafx.h"
#include <string>

struct LegendEntry
{
	char character;
	int health = 1;
	std::string effectName;
	bool invulnerable = false;
};

