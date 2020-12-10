#pragma once

#include <optional>
#include <functional>

namespace optionalUtilities
{
	// If neither of left or right have a value, returns std::nullopt. If one has a value, returns that value. If both have a value, returns the minimum as defined by the predicate.
	// Predicate: a function (T a, T b) -> bool which returns true if b is greater than a.
	template<typename T, typename Pred>
	std::optional<T>& MinValue(std::optional<T>& left, std::optional<T>& right, const Pred& predicate)
	{
		if (!left.has_value())
		{
			return right;
		}
		if (!right.has_value())
		{
			return left;
		}
		return predicate(left.value(), right.value()) ? left : right;
	}

	// If neither of left or right have a value, returns std::nullopt. If one has a value, returns that value. If both have a value, returns the maximum as defined by the predicate.
	// Predicate: a function (T a, T b) -> bool which returns true if b is greater than a.
	template<typename T, typename Pred>
	std::optional<T>& MaxValue(std::optional<T>& left, std::optional<T>& right, const Pred& predicate)
	{
		if (!left.has_value())
		{
			return right;
		}
		if (!right.has_value())
		{
			return left;
		}
		return predicate(left.value(), right.value()) ? right : left;
	}

	template<typename TIn, typename TOut>
	std::optional<TOut> Apply(std::optional<TIn> optional, std::function<TOut(TIn)> function)
	{
		return optional.has_value() ? std::optional<TOut>(function(*optional)) : std::nullopt;
	}

	// Returns whether the given optional contains a value. Overloaded template to allow uniform handling of std::optional and pointers
	template<typename T>
	bool HasValue(std::optional<T> optional)
	{
		return optional.has_value();
	}

	// Returns whether the given pointer is non-null. Overloaded template to allow uniform handling of std::optional and pointers
	template<typename T>
	bool HasValue(T* pointer)
	{
		return pointer != nullptr;
	}
}