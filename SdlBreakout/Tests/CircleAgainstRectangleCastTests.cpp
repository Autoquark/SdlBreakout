#include "stdafx.h"

#include "CircleF.h"
#include "AxisAlignedRectF.h"
#include "Assert.h"
#include "Constants.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CircleAgainstRectangleCastTests)
	{
		struct TestCase
		{
			CircleF movingCircle = CircleF(0, 0, 0);
			AxisAlignedRectF rect = AxisAlignedRectF(0, 0, 0, 0);
			Vector2F movement = Vector2F(0, 0);
			Shape::InternalityFilter internalityFilter = Shape::InternalityFilter::Both;

			std::optional<Contact> expectedResult = std::nullopt;
		};

		void RunTestCase(const TestCase& testCase)
		{
			auto hit = testCase.movingCircle.CastAgainst(testCase.rect, testCase.movement, testCase.internalityFilter);

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
				Assert::AreEqual(expected.stationarySide, actual.stationarySide);
				Assert::AreEqual(expected.distance, actual.distance);
				AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
			}
		}

	public:

		TEST_METHOD(HitLeftSideFromLeft)
		{
			TestCase testCase;
			testCase.rect = AxisAlignedRectF(0, 0, 2, 3);
			testCase.movingCircle = CircleF(-2, 1, 1);
			testCase.movement = Vector2F(3, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(0, 1), true, true, 1, Vector2F(-1, 1));

			RunTestCase(testCase);
		}

		TEST_METHOD(HitBottomInternalAtAngle)
		{
			TestCase testCase;
			testCase.rect = AxisAlignedRectF(0, 0, 10, 10);
			testCase.movingCircle = CircleF(5, 2, 1);
			testCase.movement = Vector2F(-10, -10);

			testCase.expectedResult = Contact(Vector2F(0, 1), Vector2F(4, 0), false, true, std::sqrt(2.0f), Vector2F(4, 1));

			RunTestCase(testCase);
		}
	};
}