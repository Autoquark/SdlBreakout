#include "stdafx.h"
#include <optional>
#include <vector>
#include "Vector2.h"
#include "Contact.h"
#include "RectF.h"
#include "Collision.h"
#include "ToString.h"
#include "Assert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PointRectangleCastTests)
	{
		const float Tolerance = 1e-16f;

		struct TestCase
		{
			Vector2 pointStart;
			Vector2 pointEnd;
			RectF rectangle;

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(TestCase& testCase)
		{
			auto hit = Collision::PointRectangleCast(testCase.pointStart, testCase.pointEnd, testCase.rectangle);

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

				AreEqual(expected.normal, actual.normal, Tolerance);
			}
		}

	public:
		TEST_METHOD(Miss)
		{
			TestCase testCase;
			testCase.pointStart = Vector2(0, 0);
			testCase.pointEnd = Vector2(2, 2);
			testCase.rectangle = RectF(1, -2, 2, 2);

			RunTestCase(testCase);
		}


		TEST_METHOD(HitLeftSide)
		{
			TestCase testCase;
			testCase.pointStart = Vector2(-2, 4);
			testCase.pointEnd = Vector2(1, 5);
			testCase.rectangle = RectF(0, 2, 2, 4);

			testCase.expectedResult = Contact(Vector2(-1, 0), Vector2(0, 4 + 2.0f / 3));

			RunTestCase(testCase);
		}

		TEST_METHOD(HitBottomThenRight)
		{
			TestCase testCase;
			testCase.pointStart = Vector2(2, 3);
			testCase.pointEnd = Vector2(5, 0);
			testCase.rectangle = RectF(0, 0, 4, 2);

			testCase.expectedResult = Contact(Vector2(0, 1).Normalised(), Vector2(3, 2));

			RunTestCase(testCase);
		}
	};
}