#include "stdafx.h"

#include <optional>
#include "AxisAlignedRectF.h"
#include "Contact.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{
	TEST_CLASS(RectangleAgainstRectangleCastTests)
	{
		struct TestCase
		{
			AxisAlignedRectF stationaryRectangle = AxisAlignedRectF(0, 0, 0, 0);
			AxisAlignedRectF movingRectangle = AxisAlignedRectF(0, 0, 0, 0);
			Vector2F motion = Vector2F(0, 0);

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(TestCase testCase)
		{
			auto hit = testCase.movingRectangle.CastAgainst(testCase.stationaryRectangle, testCase.motion);

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
				Assert::AreEqual(expected.stationarySide, actual.stationarySide);
				Assert::AreEqual(expected.movingSide, actual.movingSide);
				Assert::AreEqual(expected.distance, actual.distance);
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
			testCase.expectedResult = Contact(Vector2F(-1, 0),
				expectedPoint,
				true,
				true,
				1,
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
			testCase.expectedResult = Contact(Vector2F(-1, 0),
				expectedPoint,
				false,
				true,
				1,
				Vector2F(1, 0));

			RunTestCase(testCase);
		}

		TEST_METHOD(RightToLeftCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(1, 1, 2, 4);
			testCase.movingRectangle = AxisAlignedRectF(4, 4, 1, 1);
			testCase.motion = Vector2F(-2, 0);

			auto expectedPoint = Vector2F(3, 4);
			testCase.expectedResult = Contact(Vector2F(1, 0),
				expectedPoint,
				true,
				true,
				1,
				Vector2F(3.5, 4.5));

			RunTestCase(testCase);
		}

		TEST_METHOD(RightToLeftInternalCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(-2, -2, 4, 4);
			testCase.movingRectangle = AxisAlignedRectF(-1, -1, 2, 2);
			testCase.motion = Vector2F(-2, 0);

			auto expectedPoint = Vector2F(-2, 0);
			testCase.expectedResult = Contact(Vector2F(1, 0),
				expectedPoint,
				false,
				true,
				1,
				Vector2F(-1, 0));

			RunTestCase(testCase);
		}

		/*TEST_METHOD(RightToLeftEnvelopingCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(-1, -1, 2, 2);
			testCase.movingRectangle = AxisAlignedRectF(-2, -2, 4, 4);
			testCase.motion = Vector2F(-2, 0);

			auto expectedPoint = Vector2F(1, 0);
			testCase.expectedResult = Contact(Vector2F(-1, 0),
				expectedPoint,
				true,
				false,
				1,
				Vector2F(1, 0));

			RunTestCase(testCase);
		}

		TEST_METHOD(LeftToRightEnvelopingCollision)
		{
			TestCase testCase;
			testCase.stationaryRectangle = AxisAlignedRectF(-1, -1, 2, 2);
			testCase.movingRectangle = AxisAlignedRectF(-2, -2, 4, 4);
			testCase.motion = Vector2F(2, 0);

			auto expectedPoint = Vector2F(-1, 0);
			testCase.expectedResult = Contact(Vector2F(-1, 0),
				expectedPoint,
				true,
				false,
				1,
				Vector2F(1, 0));

			RunTestCase(testCase);
		}*/
	};
}