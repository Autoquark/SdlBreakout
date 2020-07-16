#pragma once

#include "stdafx.h"
#include <Vector2.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	void AreEqual(Vector2F expected, Vector2F actual, float tolerance, const wchar_t* message = nullptr);
}