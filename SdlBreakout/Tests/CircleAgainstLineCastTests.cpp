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
			Line line = Line(Vector2F(), Vector2F());
			Vector2F motion = Vector2F(0, 0);

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			for (auto swapShapes : { false, true })
			{
				for (auto swapLineEnds : { false, true })
				{
					auto& line = swapLineEnds ? Line(testCase.line.end, testCase.line.start) : testCase.line;
					auto& movement = swapShapes ? -testCase.motion : testCase.motion;
					auto& hit = swapShapes ? line.CastAgainst(testCase.circle, movement) : testCase.circle.CastAgainst(line, movement);

					if (!testCase.expectedResult.has_value())
					{
						Assert::IsFalse(hit.has_value());
					}
					else
					{
						Assert::IsTrue(hit.has_value());
						auto& expected = swapShapes ? testCase.expectedResult.value().Invert(movement) : testCase.expectedResult.value();
						auto& actual = hit.value();
						AreEqual(expected.point, actual.point, Constants::FloatEqualityTolerance);
						AreEqual(expected.centroid, actual.centroid, Constants::FloatEqualityTolerance);
						Assert::AreEqual((swapLineEnds && !swapShapes) ? !expected.stationarySide : expected.stationarySide, actual.stationarySide);
						Assert::AreEqual(expected.distance, actual.distance, Constants::FloatEqualityTolerance);

						AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
					}
				}
			}
		}

	public:
		TEST_METHOD(HitHorizontalLine)
		{
			TestCase testCase;
			testCase.line = Line(Vector2F(-2, 2), Vector2F(2, 2));
			testCase.circle = CircleF(1, 5, 1);
			testCase.motion = Vector2F(0, -10);

			testCase.expectedResult = Contact(Vector2F(0, 1), Vector2F(1, 2), false, true, 2, Vector2F(1, 3));

			RunTestCase(testCase);
		}

		TEST_METHOD(HitVerticalLine)
		{
			TestCase testCase;
			testCase.line = Line(Vector2F(2, 2), Vector2F(2, -2));
			testCase.circle = CircleF(5, 1, 1);
			testCase.motion = Vector2F(-10, 0);

			testCase.expectedResult = Contact(Vector2F(1, 0), Vector2F(2, 1), false, true, 2, Vector2F(3, 1));

			RunTestCase(testCase);
		}

		TEST_METHOD(HitDiagonalLine)
		{
			TestCase testCase;
			testCase.line = Line(Vector2F(-2, -2), Vector2F(2, 2));
			testCase.circle = CircleF(-2, 0, 1);
			testCase.motion = Vector2F(10, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 1).Normalised(),
				Vector2F(-std::sqrtf(0.5), -std::sqrtf(0.5)),
				false,
				true,
				2 - std::sqrtf(2),
				Vector2F(-std::sqrtf(2), 0));

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelToHorizontalLine)
		{
			TestCase testCase;
			testCase.line = Line(Vector2F(-2, 2), Vector2F(2, 2));
			testCase.circle = CircleF(-3, 5, 1);
			testCase.motion = Vector2F(10, 0);

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelToVerticalLine)
		{
			TestCase testCase;
			testCase.line = Line(Vector2F(2, 2), Vector2F(2, -2));
			testCase.circle = CircleF(0, -3, 1);
			testCase.motion = Vector2F(0, 10);

			RunTestCase(testCase);
		}
	};
}