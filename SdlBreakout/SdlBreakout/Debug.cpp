#include "stdafx.h"
#include "Debug.h"

#include <iostream>

void Debug::PrintChanges(const std::string& key, const std::string& value)
{
	auto previous = previousValues.find(key);
	if (previous != previousValues.end() && (*previous).second == value)
	{
		return;
	}
	std::cout << value << std::endl;
	previousValues[key] = value;
}

std::map<std::string, std::string> Debug::previousValues;
