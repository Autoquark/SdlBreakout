#pragma once
#include "stdafx.h"

#include <nlohmann/json.hpp>

class SerializableLevel
{
private:
	std::vector<std::string> grid;

public:
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLevel, grid)
};

