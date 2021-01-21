#pragma once

#include <optional>
#include <functional>

namespace optionalUtilities
{
	// If neither of left or right have a value, returns std::nullopt. If one has a value, returns that value. If both have a value, returns the minimum as defined by the predicate.
	// Predicate: a function (T a, T b) -> bool which returns true if b is greater than a.
	template<typename T, typename Pred>
	[[nodiscard]]
	std::optional<T> MinValue(const std::optional<T>& left, const std::optional<T>& right, const Pred& predicate)
	{
		std::optional<T> out;
		MinValue(left, right, predicate, out);
		return out;
	}

	template<typename T, typename Pred>
	bool MinValue(const std::optional<T>& left, const std::optional<T>& right, const Pred& predicate, std::optional<T>& out)
	{
		bool result;
		if (!right.has_value())
		{
			result = true;
		}
		else if (!left.has_value())
		{
			result = false;
		}
		else
		{
			result = predicate(left.value(), right.value());
		}

		out = result ? left : right;
		// Awkwardly, standard comparison functions(a, b) return true if a < b, but we are returning true if we picked the second argument (i.e. a > b)
		return !result;
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

	template<typename T>
	std::optional<T> Apply(std::optional<T> optional, std::function<T(T)> function)
	{
		return Apply<T, T>(optional, function);
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