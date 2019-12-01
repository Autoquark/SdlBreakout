#include <string>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SdlBreakout.Lib/Collision.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

		bool expectNull;
		Vector2 expectedResult;
	};

	TEST_CLASS(CollisionTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			TestCase testCases[1];
			testCases[0].line1Start = { 0.0, 0.0 };
			testCases[0].line1End = { 2.0, 2.0 };
			testCases[0].line2Start = { 2.0, 0.0 };
			testCases[0].line2End = { 0.0, 2.0 };
			testCases[0].expectNull = false;
			testCases[0].expectedResult = { 1.0, 1.0 };

			for (auto testCase : testCases)
			{
				auto result = Collision::LineLineIntersect(testCase.line1Start, testCase.line1End, testCase.line2Start, testCase.line2End);
				Assert::AreEqual(result == nullptr, testCase.expectNull);
				if (!testCase.expectNull)
				{
					Assert::AreEqual(*result, testCase.expectedResult);
				}
			}
			// TODO: Your test code here
		};

		

	};

}