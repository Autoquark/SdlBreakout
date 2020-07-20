#include "stdafx.h"

#include <string>
#include <optional>
#include <vector>
#include <locale>
#include <codecvt>
#include <iostream>
#include "Vector2.h"
#include "CppUnitTest.h"
#include "Collision.h"
#include "ToString.h"
#include "Contact.h"
#include "Constants.h"
#include "Assert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{	
	TEST_CLASS(PointLineCastTests)
	{
		struct TestCase
		{
			Vector2F pointStart = Vector2F();
			Vector2F pointEnd = Vector2F();
			Vector2F lineStart = Vector2F();
			Vector2F lineEnd = Vector2F();

			std::optional<PolygonContact> expectedResult;
		};

		void RunTestCase(const TestCase& testCase)
		{
			for (auto swapPointAndLine : { false, true })
			{
				for (auto swapPointEnds : { false, true })
				{
					for (auto swapLineEnds : { false, true })
					{
						auto pointStart = swapPointAndLine ? testCase.lineStart : testCase.pointStart;
						auto pointEnd = swapPointAndLine ? testCase.lineEnd : testCase.pointEnd;

						auto lineStart = swapPointAndLine ? testCase.pointStart : testCase.lineStart;
						auto lineEnd = swapPointAndLine ? testCase.pointEnd : testCase.lineEnd;

						if (swapPointEnds)
						{
							std::swap(pointStart, pointEnd);
						}

						if (swapLineEnds)
						{
							std::swap(lineStart, lineEnd);
						}

						std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

						auto hit = Collision::PointLineCast(pointStart, pointEnd, lineStart, lineEnd);
						std::wstring message = L" Test failed. Expected "s + (testCase.expectedResult.has_value() ? L""s : L"no "s) + L"collision between line from "s
							+ converter.from_bytes(pointStart.ToString()) + L" to "s + converter.from_bytes(pointEnd.ToString())
							+ L" and line from "s + converter.from_bytes(lineStart.ToString()) + L" to "s + converter.from_bytes(lineEnd.ToString());

						if (!testCase.expectedResult.has_value())
						{
							Assert::IsFalse(hit.has_value(), message.c_str());
						}
						else
						{
							Assert::IsTrue(hit.has_value(), message.c_str());
							auto &expected = testCase.expectedResult.value();
							auto &actual = hit.value();
							Assert::AreEqual(expected.point, actual.point, message.c_str());
							Assert::AreEqual(expected.centroid, actual.centroid, message.c_str());
							Assert::AreEqual((swapLineEnds ^ swapPointEnds ^ swapPointAndLine) ? !expected.side : expected.side, actual.side, message.c_str());

							// Don't check the normal if the line & point are swapped as it's not trivially calculable from the normal in the original case
							if (!swapPointAndLine)
							{
								auto expectedNormal = swapPointEnds ? -expected.normal : expected.normal;
								AreEqual(expectedNormal, actual.normal, Constants::FloatEqualityTolerance, message.c_str());
							}

						}
					}
				}
			}
		}

	public:
		
		TEST_METHOD(CrossingDiagonalLines)
		{
			TestCase testCase;

			testCase.pointStart = { 0.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 2.0, 0.0 };
			testCase.lineEnd = { 0.0, 2.0 };

			auto expectedPoint = Vector2F(1.0, 1.0);
			testCase.expectedResult = PolygonContact(Vector2F(-1, -1).Normalised(),
				expectedPoint,
				false,
				Vector2F::DistanceBetween(testCase.pointStart, expectedPoint),
				testCase.lineStart,
				testCase.lineEnd);

			RunTestCase(testCase);
		}

		TEST_METHOD(NonCrossingDiagonalLines)
		{
			// Non-crossing diagonal lines
			TestCase testCase;
			testCase.pointStart = { 0.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 3.0, 2.0 };
			testCase.lineEnd = { 5.0, 0.0 };

			testCase.expectedResult = std::nullopt;
			
			RunTestCase(testCase);
		}

		TEST_METHOD(NonOverlappingVerticalLines)
		{
			TestCase testCase;
			testCase.pointStart = { 2.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 3.0, 1.0 };
			testCase.lineEnd = { 3.0, 3.0 };

			testCase.expectedResult = std::nullopt;

			RunTestCase(testCase);
		}

		TEST_METHOD(PassingBeneathVerticalLine)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(-2, 0);
			testCase.pointEnd = Vector2F(2, 4);

			testCase.lineStart = Vector2F(0, 0);
			testCase.lineEnd = Vector2F(3, -3);

			testCase.expectedResult = std::nullopt;

			RunTestCase(testCase);
		}

		TEST_METHOD(HorizontalTrajectoryHittingVerticalLine)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(-2, 2);
			testCase.pointEnd = Vector2F(2, 2);

			testCase.lineStart = Vector2F(0, 0);
			testCase.lineEnd = Vector2F(0, 3);

			auto expectedPoint = Vector2F(0, 2);
			testCase.expectedResult = PolygonContact(Vector2F(-1, 0).Normalised(), expectedPoint, false, Vector2F::DistanceBetween(testCase.pointStart, expectedPoint), testCase.lineStart, testCase.lineEnd);

			RunTestCase(testCase);
		}

		TEST_METHOD(NonDiagonalHit)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(0, 0);
			testCase.pointEnd = Vector2F(1, 2);

			testCase.lineStart = Vector2F(1, 0);
			testCase.lineEnd = Vector2F(0, 2);

			auto expectedPoint = Vector2F(0.5, 1);
			testCase.expectedResult = PolygonContact(Vector2F(-1, -0.5).Normalised(), expectedPoint, false, Vector2F::DistanceBetween(testCase.pointStart, expectedPoint), testCase.lineStart, testCase.lineEnd);

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelHorizontalLines)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(0, 0);
			testCase.pointEnd = Vector2F(3, 0);

			testCase.lineStart = Vector2F(0, 1);
			testCase.lineEnd = Vector2F(3, 1);

			testCase.expectedResult = std::nullopt;

			RunTestCase(testCase);
		}

		TEST_METHOD(ParallelVerticalLines)
		{
			TestCase testCase;
			testCase.pointStart = Vector2F(0, 0);
			testCase.pointEnd = Vector2F(0, 3);

			testCase.lineStart = Vector2F(1, 0);
			testCase.lineEnd = Vector2F(1, 3);

			testCase.expectedResult = std::nullopt;

			RunTestCase(testCase);
		}

		// Overlapping vertical lines
		// Don't know what if anything in particular is a useful return value here
		/*i++;
		testCases[i].pointStart = { 2.0, 0.0 };
		testCases[i].pointEnd = { 2.0, 2.0 };

		testCases[i].lineStart = { 2.0, 1.0 };
		testCases[i].lineEnd = { 2.0, 3.0 };

		testCases[i].expectedResult = Contact(Vector2F{ 0, 0 }, Vector2F { 2.0, 1.0 });*/
	};

}