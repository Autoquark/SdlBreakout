#include "stdafx.h"

#include "CircleF.h"
#include "AxisAlignedRectF.h"
#include "Assert.h"
#include "Constants.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CircleAgainstCircleCastTests)
	{
		struct TestCase
		{
			CircleF movingCircle = CircleF(0, 0, 0);
			CircleF stationaryCircle = CircleF(0, 0, 0);
			Vector2F movement = Vector2F(0, 0);
			Shape::InternalityFilter internalityFilter = Shape::InternalityFilter::Both;

			std::optional<Contact> expectedResult = std::nullopt;
		};

		void RunTestCase(const TestCase& testCase)
		{
			for (auto swapShapes : { false, true })
			{
				auto hit = swapShapes ? testCase.stationaryCircle.CastAgainst(testCase.movingCircle, -testCase.movement, testCase.internalityFilter)
					: testCase.movingCircle.CastAgainst(testCase.stationaryCircle, testCase.movement, testCase.internalityFilter);

				if (!testCase.expectedResult.has_value())
				{
					Assert::IsFalse(hit.has_value());
				}
				else
				{
					Assert::IsTrue(hit.has_value());
					auto& expected = swapShapes ? testCase.expectedResult->Invert(testCase.movement, testCase.stationaryCircle.centre) : testCase.expectedResult.value();
					auto& actual = hit.value();
					Assert::AreEqual(expected.point, actual.point);
					Assert::AreEqual(expected.centroid, actual.centroid);
					Assert::AreEqual(expected.stationarySide, actual.stationarySide);
					Assert::AreEqual(expected.distance, actual.distance);
					AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);

					Assert::AreEqual(testCase.movingCircle.radius, Vector2F::DistanceBetween(actual.centroid, actual.point));
				}
			}
		}

	public:

		TEST_METHOD(HitLeftSideFromRight)
		{
			TestCase testCase;
			testCase.stationaryCircle = CircleF(1, 0, 1);
			testCase.movingCircle = CircleF(-2, 0, 1);
			testCase.movement = Vector2F(3, 0);
			testCase.internalityFilter = Shape::InternalityFilter::External;

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(0, 0), true, true, 1, Vector2F(-1, 0));

			RunTestCase(testCase);
		}
	};
}