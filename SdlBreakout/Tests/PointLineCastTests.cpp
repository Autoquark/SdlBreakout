#include "stdafx.h"

#include <string>
#include <optional>
#include <vector>
#include "Vector2.h"
#include "CppUnitTest.h"
#include "Collision.h"
#include "ToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{	
	struct TestCase
	{
		Vector2 line1Start;
		Vector2 line1End;
		Vector2 line2Start;
		Vector2 line2End;

		std::optional<Contact> expectedResult;
	};

	TEST_CLASS(PointLineCastTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::vector<TestCase> testCases;
			TestCase testCase;

			// Crossing diagonal lines
			testCase.line1Start = { 0.0, 0.0 };
			testCase.line1End = { 2.0, 2.0 };

			testCase.line2Start = { 2.0, 0.0 };
			testCase.line2End = { 0.0, 2.0 };

			auto normal = Vector2{ -1, -1 };
			normal.Normalise();
			testCase.expectedResult = Contact(normal, Vector2{ 1.0, 1.0 }, Vector2{ 1.0, 1.0 });
			testCases.emplace_back(testCase);

			// Non-crossing diagonal lines
			testCase = TestCase();
			testCase.line1Start = { 0.0, 0.0 };
			testCase.line1End = { 2.0, 2.0 };

			testCase.line2Start = { 3.0, 2.0 };
			testCase.line2End = { 5.0, 0.0 };

			testCase.expectedResult = std::nullopt;
			testCases.emplace_back(testCase);

			// Non-overlapping vertical lines
			testCase.line1Start = { 2.0, 0.0 };
			testCase.line1End = { 2.0, 2.0 };

			testCase.line2Start = { 3.0, 1.0 };
			testCase.line2End = { 3.0, 3.0 };

			testCase.expectedResult = std::nullopt;
			testCases.emplace_back(testCase);

			// Overlapping vertical lines
			// Don't know what if anything in particular is a useful return value here
			/*i++;
			testCases[i].line1Start = { 2.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 2.0, 1.0 };
			testCases[i].line2End = { 2.0, 3.0 };

			testCases[i].expectedResult = Contact(Vector2{ 0, 0 }, Vector2 { 2.0, 1.0 }, Vector2{ 2.0, 1.0 });*/

			for (auto testCase : testCases)
			{
				for (auto swapLines : { false, true})
				{
					for (auto swapLine1Ends : { false, true })
					{
						for (auto swapLine2Ends : { false, true })
						{
							auto line1Start = swapLines ? testCase.line2Start : testCase.line1Start;
							auto line1End = swapLines ? testCase.line2End : testCase.line1End;

							auto line2Start = swapLines ? testCase.line1Start : testCase.line2Start;
							auto line2End = swapLines ? testCase.line1End : testCase.line2End;

							if (swapLine1Ends)
							{
								std::swap(line1Start, line1End);
							}

							if (swapLine2Ends)
							{
								std::swap(line2Start, line2End);
							}

							auto hit = Collision::PointLineCast(line1Start, line1End, line2Start, line2End);
							std::string message = "Expected " + (testCase.expectedResult.has_value() ? ""s : "no "s) + "collision between line from "s  + line1Start.ToString() + " to " + line1End.ToString()
								+ " and line from " + line2Start.ToString() + " to " + line2End.ToString();

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

								// Don't check the normal if the line & point are swapped as it's not trivially calculable from the normal in the original case
								if (!swapLines)
								{
									Assert::AreEqual(swapLine1Ends ? -expected.normal : expected.normal, actual.normal);
								}

							}
							if (testCase.expectedResult != hit)
							{
								__asm nop
							}
						}
					}
				}
			}
		};
	};

}