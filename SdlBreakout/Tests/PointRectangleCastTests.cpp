#include "stdafx.h"
#include <optional>
#include <vector>
#include "Vector2.h"
#include "Contact.h"
#include "RectF.h"
#include "Collision.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(PointRectangleCastTests)
	{
		struct TestCase
		{
			Vector2 pointStart;
			Vector2 pointEnd;
			RectF rectangle;

			std::optional<Contact> expectedResult;
		};

	public:

		TEST_METHOD(PointRectangleCast)
		{
			std::vector<TestCase> testCases;
			TestCase testCase;

			// Miss
			testCase.pointStart = Vector2(0, 0);
			testCase.pointEnd = Vector2(2, 2);
			testCase.rectangle = RectF(1, -2, 2, 2);
			testCases.emplace_back(testCase);

			// Hit left side
			testCase = TestCase();
			testCase.pointStart = Vector2(-2, 0);
			testCase.pointEnd = Vector2(1, 1);
			testCase.rectangle = RectF(0, 2, 2, 4);

			testCase.expectedResult = Contact(Vector2(-1, 0), Vector2(0, 2.0 / 3));
			testCases.emplace_back(testCase);

			// Hit bottom then right
			testCase = TestCase();
			testCase.pointStart = Vector2(2, 3);
			testCase.pointEnd = Vector2(5, 0);
			testCase.rectangle = RectF(0, 0, 4, 2);

			testCase.expectedResult = Contact(Vector2(-1, -1).Normalised(), Vector2(3, 2));
			testCases.emplace_back(testCase);

			for (auto testCase : testCases)
			{
				for (auto swapPointEnds : { false, true })
				{
					auto pointStart = swapPointEnds ? testCase.pointEnd : testCase.pointStart;
					auto pointEnd = swapPointEnds ? testCase.pointStart : testCase.pointEnd;

					auto hit = Collision::PointRectangleCast(pointStart, pointEnd, testCase.rectangle);

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

						Assert::AreEqual(swapPointEnds ? -expected.normal : expected.normal, actual.normal);
					}
				}
			}
		};
	};
}