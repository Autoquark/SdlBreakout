#pragma once

#include <optional>

namespace optionalUtilities
{
	template<typename T>
	bool HasValue(std::optional<T> optional)
	{
		return optional.has_value();
	}

	template<typename T>
	bool HasValue(T* pointer)
	{
		return pointer != nullptr;
	}
}