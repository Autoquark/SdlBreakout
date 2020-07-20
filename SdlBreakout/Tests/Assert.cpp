#include "stdafx.h"

#include <optional>
#include "Assert.h"
#include "Contact.h"


namespace Tests
{
	void AreEqual(const Vector2F& expected, const Vector2F& actual, const float tolerance, const wchar_t* message)
	{
		Assert::AreEqual(expected.x, actual.x, tolerance, message);
		Assert::AreEqual(expected.y, actual.y, tolerance, message);
	}

	//template<typename T>
	//void AreEqual(const)

	/*void AreEqual(const Contact& expected, const Contact& actual)
	{

	}*/
}