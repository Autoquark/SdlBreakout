#include <string>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SdlBreakout.Lib/Collision.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<Vector2>(const Vector2& t)
			{
				std::wstring value = L"(";
				value.append(std::to_wstring(t.x));
				value.append(L",");
				value.append(std::to_wstring(t.y));
				value.append(L")");
				return value;
			}
		}
	}
}

namespace Tests
{	
	struct TestCase
	{
		Vector2 line1Start;
		Vector2 line1End;
		Vector2 line2Start;
		Vector2 line2End;

		bool expectCollision;
		Vector2 expectedResult;
	};

	TEST_CLASS(CollisionTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			TestCase testCases[4];

			auto i = 0;
			// Crossing diagonal lines
			testCases[i].line1Start = { 0.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 2.0, 0.0 };
			testCases[i].line2End = { 0.0, 2.0 };

			testCases[i].expectCollision = true;
			testCases[i].expectedResult = { 1.0, 1.0 };

			// Non-crossing diagonal lines
			i++;
			testCases[i].line1Start = { 0.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 3.0, 2.0 };
			testCases[i].line2End = { 5.0, 0.0 };

			testCases[i].expectCollision = false;

			// Non-overlapping vertical lines
			i++;
			testCases[i].line1Start = { 2.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 3.0, 1.0 };
			testCases[i].line2End = { 3.0, 3.0 };

			testCases[i].expectCollision = false;

			// Overlapping vertical lines
			i++;
			testCases[i].line1Start = { 2.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 2.0, 1.0 };
			testCases[i].line2End = { 2.0, 3.0 };

			testCases[i].expectCollision = true;
			testCases[i].expectedResult = { 2.0, 1.0 };

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
								std::swap(line1Start, line1End);
							}

							Vector2 result;
							auto hit = Collision::LineLineIntersect(line1Start, line1End, line2Start, line2End, result);
							std::string message = "Expected " + (testCase.expectCollision ? ""s : "no "s) + "collision between line from "s  + line1Start.ToString() + " to " + line1End.ToString()
								+ " and line from " + line2Start.ToString() + " to " + line2End.ToString();
							//Assert::AreEqual(hit, testCase.expectCollision);
							if (hit != testCase.expectCollision)
							{
								__asm nop
							}
							if (testCase.expectCollision)
							{
								//Assert::AreEqual(testCase.expectedResult, result);
								if (testCase.expectedResult != result)
								{
									__asm nop
								}
							}
						}
					}
				}
				
			}
			// TODO: Your test code here
		};

		

	};

}