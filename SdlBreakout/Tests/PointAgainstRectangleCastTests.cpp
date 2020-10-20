#include "stdafx.h"
#include <optional>
#include <vector>
#include "Vector2.h"
#include "Contact.h"
#include "AxisAlignedRectF.h"
#include "Collision.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"
#include "Point.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PointAgainstRectangleCastTests)
	{
		struct TestCase
		{
			Vector2F pointStart = Vector2F();
			Vector2F pointEnd = Vector2F();
			AxisAlignedRectF rectangle = AxisAlignedRectF(0, 0, 0, 0);
			Shape::InternalityFilter internalityFilter = Shape::InternalityFilter::Both;

			std::optional<PolygonContact> expectedResult;
		};

		void RunTestCase(TestCase& testCase)
		{
			// Don't swap line ends as this might result in hitting a different side of the rectangle first
			auto hit = Point(testCase.pointStart).CastAgainst(testCase.rectangle, testCase.pointEnd - testCase.pointStart, testCase.internalityFilter);
			//auto hit = Collision::PointRectangleCast(testCase.pointStart, testCase.pointEnd, testCase.rectangle, testCase.internalityFilter);
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
		TEST_METHOD(Miss)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(0, 0);
			testCase.pointEnd = Vector2F(2, 2);
			testCase.rectangle = AxisAlignedRectF(1, -2, 2, 2);
			testCase.internalityFilter = Shape::InternalityFilter::Both;

			RunTestCase(testCase);
		}


		TEST_METHOD(HitLeftSide)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(-2, 4);
			testCase.pointEnd = Vector2F(1, 5);
			testCase.rectangle = AxisAlignedRectF(0, 2, 2, 4);
			testCase.internalityFilter = Shape::InternalityFilter::Both;

			auto expectedPoint = Vector2F(0, 4 + 2.0f / 3);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0),
				expectedPoint,
				true,
				true,
				Vector2F::DistanceBetween(testCase.pointStart, expectedPoint),
				testCase.rectangle.BottomLeft(),
				testCase.rectangle.TopLeft());

			RunTestCase(testCase);
		}

		TEST_METHOD(HitLeftSide2)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(0.5, 4.5);
			testCase.pointEnd = Vector2F(2, 3);
			testCase.rectangle = AxisAlignedRectF(1, 1, 2, 4);
			testCase.internalityFilter = Shape::InternalityFilter::Both;

			auto expectedPoint = Vector2F(1, 4);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0),
				expectedPoint,
				true,
				true,
				Vector2F::DistanceBetween(testCase.pointStart, expectedPoint),
				testCase.rectangle.BottomLeft(),
				testCase.rectangle.TopLeft());

			RunTestCase(testCase);
		}

		TEST_METHOD(HitBottomThenRight)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(2, 3);
			testCase.pointEnd = Vector2F(5, 0);
			testCase.rectangle = AxisAlignedRectF(0, 0, 4, 2);
			testCase.internalityFilter = Shape::InternalityFilter::Both;

			auto expectedPoint = Vector2F(3, 2);
			testCase.expectedResult = PolygonContact(Vector2F(0, 1).Normalised(),
				expectedPoint,
				true,
				true,
				Vector2F::DistanceBetween(testCase.pointStart, expectedPoint),
				testCase.rectangle.BottomRight(),
				testCase.rectangle.BottomLeft());

			RunTestCase(testCase);
		}

		TEST_METHOD(HitBottomThenRightFilterInternal)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(2, 3);
			testCase.pointEnd = Vector2F(5, 0);
			testCase.rectangle = AxisAlignedRectF(0, 0, 4, 2);
			testCase.internalityFilter = Shape::InternalityFilter::Internal;

			auto expectedPoint = Vector2F(4, 1);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0).Normalised(),
				expectedPoint,
				false,
				true,
				Vector2F::DistanceBetween(testCase.pointStart, expectedPoint),
				testCase.rectangle.TopRight(),
				testCase.rectangle.BottomRight());

			RunTestCase(testCase);
		}
	};
}