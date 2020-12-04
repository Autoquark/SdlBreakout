#include "stdafx.h"

#include <codecvt>

#include "Vector2.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(ReflectTests)
	{
		struct TestCase
		{
			Vector2F vector;
			Vector2F reflectionVector;

			Vector2F expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

			float rotations[] = { 0, 45, 90, 135, 180, 225, 270, 315 };
			for (auto rotation : rotations)
			{
				auto expected = testCase.expectedResult.Rotated(rotation);
				auto result = testCase.vector.Rotated(rotation).Reflected(testCase.reflectionVector.Rotated(rotation));

				std::wstring message = L"Failed for rotation "s + converter.from_bytes(std::to_string(rotation));
				AreEqual(expected, result, Constants::FloatEqualityTolerance, message.c_str());
			}
			
		}

		TEST_METHOD(Perpendicular)
		{
			TestCase testCase;
			testCase.vector = Vector2F(0, 1);
			testCase.reflectionVector = Vector2F(0, -1);
			testCase.expectedResult = Vector2F(0, -1);

			RunTestCase(testCase);
		}

		TEST_METHOD(At45Degrees)
		{
			TestCase testCase;
			testCase.vector = Vector2F(0, 1);
			testCase.reflectionVector = Vector2F(1, 1);
			testCase.expectedResult = Vector2F(-1, 0);

			RunTestCase(testCase);
		}
	};
}