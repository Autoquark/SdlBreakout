#pragma once
#include "stdafx.h"

#include <vector>
#include <string>

class Menu
{
public:
	void Show(const std::vector<std::string> options)
	{
		if (options.size() == 0)
		{
			throw new std::exception();
		}
		this->options = options;
	}

	int Update();

private:
	int selectedIndex = 0;
	std::vector<std::string> options;
};

