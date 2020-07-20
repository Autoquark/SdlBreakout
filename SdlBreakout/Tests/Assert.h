#pragma once

#include "stdafx.h"
#include "Vector2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	void AreEqual(const Vector2F& expected, const Vector2F& actual, const float tolerance, const wchar_t* message = NULL);
	//void AreEqual(const Contact& expected, const Contact& actual);
}