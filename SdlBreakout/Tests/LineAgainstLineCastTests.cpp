#include "stdafx.h"

#include "Line.h"
#include "Constants.h"
#include "Assert.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{
	TEST_CLASS(LineAgainstLineCastTests)
	{
		struct TestCase
		{
			Line stationaryLine = Line(Vector2F(), Vector2F());
			Line movingLine = Line(Vector2F(), Vector2F());

			Vector2F movement = Vector2F();
			Shape::InternalityFilter internalityFilter = Shape::InternalityFilter::Both;

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			for (auto swapLines : { false, true })
			{
				for (auto swapLine1Ends : { false, true })
				{
					for (auto swapLine2Ends : { false, true })
					{
						auto stationaryLine = testCase.stationaryLine;
						auto movingLine = testCase.movingLine;

						if (swapLines)
						{
							std::swap(stationaryLine, movingLine);
						}

						if (swapLine1Ends)
						{
							std::swap(stationaryLine.start, stationaryLine.end);
						}

						if (swapLine2Ends)
						{
							std::swap(movingLine.start, movingLine.end);
						}

						auto hit = movingLine.CastAgainst(stationaryLine, swapLines ? -testCase.movement : testCase.movement, testCase.internalityFilter);
						if (!testCase.expectedResult.has_value())
						{
							Assert::IsFalse(hit.has_value());
						}
						else
						{
							Assert::IsTrue(hit.has_value());
							auto& expected = swapLines ? testCase.expectedResult->Invert(testCase.movement, testCase.stationaryLine.GetCentre()) : testCase.expectedResult.value();
							auto& actual = hit.value();
							Assert::AreEqual(expected.point, actual.point);
							Assert::AreEqual(expected.centroid, actual.centroid);
							Assert::AreEqual(swapLine1Ends != expected.stationarySide, actual.stationarySide);
							Assert::AreEqual(expected.distance, actual.distance);
							AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
						}
					}
				}
			}
		}

	public:
		TEST_METHOD(SlantedMoveRightIntoVertical)
		{
			TestCase testCase;
			testCase.stationaryLine = Line(Vector2F(0, -2), Vector2F(0, 2));
			testCase.movingLine = Line(Vector2F(-1, -1), Vector2F(-2, 1));
			testCase.movement = Vector2F(10, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(0, -1), true, false, 1);
			testCase.expectedResult->centroid = testCase.movingLine.GetCentre() + testCase.movement.WithMagnitude(testCase.expectedResult->distance);

			RunTestCase(testCase);
		}
	};
}