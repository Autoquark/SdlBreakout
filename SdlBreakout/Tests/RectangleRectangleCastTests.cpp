#include "stdafx.h"

#include <optional>
#include "AxisAlignedRectF.h"
#include "Contact.h"
#include "Collision.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{
	TEST_CLASS(RectangleRectangleCastTests)
	{
		struct TestCase
		{
			AxisAlignedRectF stationaryRectangle;
			AxisAlignedRectF movingRectangle;
			Vector2F motion;

			std::optional<PolygonContact> expectedResult;
		};

		void RunTestCase(TestCase testCase)
		{
			auto hit = Collision::RectangleRectangleCast(testCase.movingRectangle, testCase.stationaryRectangle, testCase.motion);

			if (!testCase.expectedResult.has_value())
			{
				Assert::IsFalse(hit.has_value());
			}
			else
			{
				Assert::IsTrue(hit.has_value());
				auto &expected = testCase.expectedResult.value();
				auto &actual = hit.value();
				Assert::AreEqual(expected.point, actual.point);
				Assert::AreEqual(expected.centroid, actual.centroid);
				Assert::AreEqual(expected.side, actual.side);
				AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
			}
		}

	public:
		TEST_METHOD(LeftToRightCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(1, 1, 2, 4);
			testCase.movingRectangle = AxisAlignedRectF(-1, 4, 1, 1);
			testCase.motion = Vector2F(2, 0);

			auto expectedPoint = Vector2F(1, 4);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0),
				expectedPoint,
				true,
				Vector2F::DistanceBetween(testCase.movingRectangle.Centre(), expectedPoint), //TODO: Centroid, not point
				testCase.stationaryRectangle.BottomLeft(),
				testCase.stationaryRectangle.TopLeft(),
				Vector2F(0.5, 4.5));

			RunTestCase(testCase);
		}

		TEST_METHOD(LeftToRightInternalCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(-2, -2, 4, 4);
			testCase.movingRectangle = AxisAlignedRectF(-1, -1, 2, 2);
			testCase.motion = Vector2F(2, 0);

			auto expectedPoint = Vector2F(2, 0);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0),
				expectedPoint,
				false,
				Vector2F::DistanceBetween(testCase.movingRectangle.Centre(), expectedPoint),
				testCase.stationaryRectangle.TopRight(),
				testCase.stationaryRectangle.BottomRight(),
				Vector2F(1, 0));

			RunTestCase(testCase);
		}
	};
}