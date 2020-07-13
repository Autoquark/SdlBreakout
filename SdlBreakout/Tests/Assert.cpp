#include "stdafx.h"

#include "Assert.h"

namespace Tests
{
	void AreEqual(Vector2 expected, Vector2 actual, float tolerance, const wchar_t* message)
	{
		Assert::AreEqual(expected.x, actual.x, tolerance, message);
		Assert::AreEqual(expected.y, actual.y, tolerance, message);
	}
}