#include "stdafx.h"

#include "CircleF.h"
#include "ToString.h"
#include "Constants.h"
#include "Assert.h"
#include "Point.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PointAgainstCircleCastTests)
	{
		struct TestCase
		{
			Vector2F pointStart = Vector2F();
			Vector2F pointEnd = Vector2F();
			CircleF circle = CircleF(Vector2F(0, 0), 1);
			Shape::InternalityFilter internalityFilter = Shape::InternalityFilter::Both;

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			auto hit = Point(testCase.pointStart).CastAgainst(testCase.circle, testCase.pointEnd - testCase.pointStart, testCase.internalityFilter);
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
		TEST_METHOD(OneHitLeftToRight)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(-2, 0);
			testCase.pointEnd = Vector2F(0, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(-1, 0), true, true, 1);

			RunTestCase(testCase);
		}

		TEST_METHOD(LeftToRightFilteredOut)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(-2, 0);
			testCase.pointEnd = Vector2F(0, 0);
			testCase.internalityFilter = Shape::InternalityFilter::Internal;

			RunTestCase(testCase);
		}

		TEST_METHOD(InternalHitLeftToRight)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(0, 0);
			testCase.pointEnd = Vector2F(2, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(1, 0), false, true, 1);

			RunTestCase(testCase);
		}

		TEST_METHOD(PierceLeftToRight)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(-2, 0);
			testCase.pointEnd = Vector2F(2, 0);

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(-1, 0), true, true, 1);

			RunTestCase(testCase);
		}

		TEST_METHOD(PierceLeftToRightFilterInternal)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(-2, 0);
			testCase.pointEnd = Vector2F(2, 0);
			testCase.internalityFilter = Shape::InternalityFilter::Internal;

			testCase.expectedResult = Contact(Vector2F(-1, 0), Vector2F(1, 0), false, true, 3);

			RunTestCase(testCase);
		}

		TEST_METHOD(MissAbove)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(0, 0), 1);
			testCase.pointStart = Vector2F(-2, 2);
			testCase.pointEnd = Vector2F(2, 2);

			RunTestCase(testCase);
		}

		TEST_METHOD(FallShort)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(2, 2), 2);
			testCase.pointStart = Vector2F(-6, 0);
			testCase.pointEnd = Vector2F(-1, 0);

			RunTestCase(testCase);
		}

		TEST_METHOD(FallShortInternal)
		{
			TestCase testCase;
			testCase.circle = CircleF(Vector2F(2, 2), 2);
			testCase.pointStart = Vector2F(1, 1);
			testCase.pointEnd = Vector2F(3, 3);

			RunTestCase(testCase);
		}
	};
}