#pragma once
#include "stdafx.h"

#include <nlohmann/json.hpp>

#include "LegendEntry.h"

struct SerializableLevel
{
public:
	std::vector<std::string> grid;
	std::vector<LegendEntry> legend;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLevel, grid, legend)
};

