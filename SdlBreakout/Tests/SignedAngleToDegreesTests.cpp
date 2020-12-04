#include "stdafx.h"

#include "Vector2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(SignedAngleToDegreesTests)
	{
		struct TestCase
		{
			Vector2F vector;
			Vector2F vector2;

			float expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			auto result = testCase.vector.SignedAngleToDegrees(testCase.vector2);
			Assert::AreEqual(testCase.expectedResult, result);

			result = testCase.vector2.SignedAngleToDegrees(testCase.vector);
			Assert::AreEqual(-testCase.expectedResult, result);
		}

		TEST_METHOD(RightDown)
		{
			TestCase testCase;
			testCase.vector = Vector2F(1, 0);
			testCase.vector2 = Vector2F(0, 1);
			testCase.expectedResult = 90;

			RunTestCase(testCase);
		}

		TEST_METHOD(LeftDown)
		{
			TestCase testCase;
			testCase.vector = Vector2F(-1, 0);
			testCase.vector2 = Vector2F(0, 1);
			testCase.expectedResult = -90;

			RunTestCase(testCase);
		}

		TEST_METHOD(LeftRight)
		{
			TestCase testCase;
			testCase.vector = Vector2F(-1, 0);
			testCase.vector2 = Vector2F(1, 0);
			testCase.expectedResult = -180;

			RunTestCase(testCase);
		}

		TEST_METHOD(UpLeftUpRight)
		{
			TestCase testCase;
			testCase.vector = Vector2F(-1, -1);
			testCase.vector2 = Vector2F(1, -1);
			testCase.expectedResult = 90;

			RunTestCase(testCase);
		}
	};
}