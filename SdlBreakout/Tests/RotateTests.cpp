#include "stdafx.h"

#include "Vector2.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(RotateTests)
	{
		struct TestCase
		{
			Vector2F vector;
			float rotation;

			Vector2F expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			auto result = testCase.vector.Rotated(testCase.rotation);
			AreEqual(testCase.expectedResult, result, Constants::FloatEqualityTolerance);
		}

		TEST_METHOD(Up45)
		{
			TestCase testCase;
			testCase.vector = Vector2F(0, 1);
			testCase.rotation = 45;
			testCase.expectedResult = Vector2F(1 / std::sqrtf(2), 1 / std::sqrtf(2));

			RunTestCase(testCase);
		}

		TEST_METHOD(Up90)
		{
			TestCase testCase;
			testCase.vector = Vector2F(0, 1);
			testCase.rotation = 90;
			testCase.expectedResult = Vector2F(1, 0);

			RunTestCase(testCase);
		}
	};
}