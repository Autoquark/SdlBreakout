#pragma once
#include "stdafx.h"
#include <string>
#include <nlohmann/json.hpp>

struct LegendEntry
{
	std::string character;
	int health = 1;
	std::string effectName;
	bool invulnerable = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(LegendEntry, character, health, effectName, invulnerable)
};

