#pragma once
#include "stdafx.h"

#include <nlohmann/json.hpp>

struct SerializableLevel
{
public:
	std::vector<std::string> grid;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLevel, grid)
};

