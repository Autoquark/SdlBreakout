#include "stdafx.h"

#include <optional>
#include "Contact.h"
#include "ToString.h"
#include "Assert.h"
#include "Constants.h"
#include "Point.h"
#include "Polygon.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{
	TEST_CLASS(PointAgainstPolygonCastTests)
	{
		struct TestCase
		{
			Point stationaryPoint = Point(0, 0);
			Polygon movingPolygon = Polygon(std::vector<Vector2F>());
			Vector2F motion = Vector2F(0, 0);

			std::optional<Contact> expectedResult;
		};

		void RunTestCase(TestCase testCase)
		{
			for (auto swapShapes : { false, true })
			{
				auto hit = swapShapes ? testCase.stationaryPoint.CastAgainst(testCase.movingPolygon, -testCase.motion)
					: testCase.movingPolygon.CastAgainst(testCase.stationaryPoint, testCase.motion);

				if (!testCase.expectedResult.has_value())
				{
					Assert::IsFalse(hit.has_value());
				}
				else
				{
					Assert::IsTrue(hit.has_value());
					const auto& expected = swapShapes ? testCase.expectedResult->Invert(testCase.motion, testCase.stationaryPoint) : testCase.expectedResult.value();
					auto& actual = hit.value();
					Assert::AreEqual(expected.point, actual.point);
					Assert::AreEqual(expected.centroid, actual.centroid);
					Assert::AreEqual(expected.stationarySide, actual.stationarySide);
					Assert::AreEqual(expected.movingSide, actual.movingSide);
					Assert::AreEqual(expected.distance, actual.distance);
					AreEqual(expected.normal, actual.normal, Constants::FloatEqualityTolerance);
				}
			}
		}

	public:
		TEST_METHOD(SquareRightToLeftCollision)
		{
			TestCase testCase;
			testCase.stationaryPoint = Point(0, 0);
			auto vertices = std::vector<Vector2F>
			{
				Vector2F(1, -1),
				Vector2F(2, -1),
				Vector2F(2, 1),
				Vector2F(1, 1),
			};
			testCase.movingPolygon = Polygon(vertices);
			testCase.motion = Vector2F(-2, 0);

			auto expectedPoint = Vector2F(0, 0);
			auto distance = 1.0f;
			testCase.expectedResult = Contact(Vector2F(1, 0),
				expectedPoint,
				true,
				true,
				distance,
				testCase.movingPolygon.GetCentre() + distance * testCase.motion.Normalised());

			RunTestCase(testCase);
		}
	};
}