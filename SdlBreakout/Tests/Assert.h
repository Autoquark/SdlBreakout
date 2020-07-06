#pragma once

#include "stdafx.h"
#include <Vector2.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void AreEqual(Vector2 first, Vector2 second, float tolerance, const wchar_t* message = nullptr)
{
	Assert::AreEqual(first.x, second.x, tolerance, message);
	Assert::AreEqual(first.y, second.y, tolerance, message);
}