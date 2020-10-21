#include "stdafx.h"

#include "CircleF.h"
#include "ToString.h"
#include "FloatCompare.h"
#include "Assert.h"
#include "Constants.h"
#include "Line.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CircleAgainstLineCastTests)
	{
		struct TestCase
		{
			CircleF circle = CircleF(0, 0, 0);
			Vector2F lineStart = Vector2F(0, 0);
			Vector2F lineEnd = Vector2F(0, 0);
			Vector2F motion = Vector2F(0, 0);

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			for (auto swapLineEnds : { false, true })
			{
				auto hit = testCase.circle.CastAgainst(Line(swapLineEnds ? testCase.lineEnd : testCase.lineStart, swapLineEnds ? testCase.lineStart : testCase.lineEnd), testCase.motion);
				//auto hit = Collision::CircleLineCast(testCase.circle, swapLineEnds ? testCase.lineEnd : testCase.lineStart, swapLineEnds ? testCase.lineStart : testCase.lineEnd, testCase.motion);
				if (!testCase.expectedResult.has_value())
				{
					Assert::IsFalse(hit.has_value());
				}
				else
				{
					Assert::IsTrue(hit.has_value());
					auto& expected = testCase.expectedResult.value();
					auto& actual = hit.value();
					Assert::AreEqual(expected.point, actual.point);
					Assert::AreEqual(expected.centroid, actual.centroid);
					Assert::AreEqual(swapLineEnds ? !expected.stationarySide : expected.stationarySide, actual.stationarySide);
					Assert::AreEqual(expected.distance, actual.distance);

					AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
				}
			}
		}

	public:
		TEST_METHOD(HitHorizontalLine)
		{
			TestCase testCase;
			testCase.lineStart = Vector2F(-2, 2);
			testCase.lineEnd = Vector2F(2, 2);
			testCase.circle = CircleF(1, 5, 1);
			testCase.motion = Vector2F(0, -10);

			testCase.expectedResult = Contact(Vector2F(0, 1), Vector2F(1, 2), false, true, 2, Vector2F(1, 3));

			RunTestCase(testCase);
		}

		TEST_METHOD(HitVerticalLine)
		{
			TestCase testCase;
			testCase.lineStart = Vector2F(2, 2);
			testCase.lineEnd = Vector2F(2, -2);
			testCase.circle = CircleF(5, 1, 1);
			testCase.motion = Vector2F(-10, 0);

			testCase.expectedResult = Contact(Vector2F(1, 0), Vector2F(2, 1), false, true, 2, Vector2F(3, 1));

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelToHorizontalLine)
		{
			TestCase testCase;
			testCase.lineStart = Vector2F(-2, 2);
			testCase.lineEnd = Vector2F(2, 2);
			testCase.circle = CircleF(-3, 5, 1);
			testCase.motion = Vector2F(10, 0);

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelToVerticalLine)
		{
			TestCase testCase;
			testCase.lineStart = Vector2F(2, 2);
			testCase.lineEnd = Vector2F(2, -2);
			testCase.circle = CircleF(0, -3, 1);
			testCase.motion = Vector2F(0, 10);

			RunTestCase(testCase);
		}
	};
}