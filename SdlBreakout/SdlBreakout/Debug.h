#pragma once
#include "stdafx.h"

#include <string>
#include <map>

#include "Vector2.h"

class Debug
{
public:
	static bool condition1;

	template<typename T>
	static void PrintChanges(const std::string& key, const T& value)
	{
		PrintChanges(key, ToString(value));
	}
	static void PrintChanges(const std::string& key, const std::string& value);

	template<typename T>
	static std::string ToString(const T& value)
	{
		return std::to_string(value);
	}
	template<>
	static std::string ToString(const Vector2F& value)
	{
		return value.ToString();
	}

private:
	Debug() = delete;

	static std::map<std::string, std::string> previousValues;
};

