#include "stdafx.h"

#include <string>
#include <optional>
#include "Vector2.h"
#include "CppUnitTest.h"
#include "Collision.h"

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

			template<> inline std::wstring ToString<Contact>(const Contact& t)
			{
				std::wstring value = L"(point: ";
				value.append(ToString(t.point));
				value.append(L", centroid: ");
				value.append(ToString(t.centroid));
				value.append(L", normal: ");
				value.append(ToString(t.normal));
				value.append(L")");
				return value;
			}


			template<typename T> inline std::wstring ToString(const std::optional<T>& t)
			{
				return t.has_value() ? ToString(t.value()) : L"null";
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

		std::optional<Contact> expectedResult;
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

			testCases[i].expectedResult = Contact(Vector2{ -1, -1 }, Vector2 { 1.0, 1.0 }, Vector2{ 1.0, 1.0 });

			// Non-crossing diagonal lines
			i++;
			testCases[i].line1Start = { 0.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 3.0, 2.0 };
			testCases[i].line2End = { 5.0, 0.0 };

			testCases[i].expectedResult = std::nullopt;

			// Non-overlapping vertical lines
			i++;
			testCases[i].line1Start = { 2.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 3.0, 1.0 };
			testCases[i].line2End = { 3.0, 3.0 };

			testCases[i].expectedResult = std::nullopt;

			// Overlapping vertical lines
			i++;
			testCases[i].line1Start = { 2.0, 0.0 };
			testCases[i].line1End = { 2.0, 2.0 };

			testCases[i].line2Start = { 2.0, 1.0 };
			testCases[i].line2End = { 2.0, 3.0 };

			testCases[i].expectedResult = Contact(Vector2{ 0, 0 }, Vector2 { 2.0, 1.0 }, Vector2{ 2.0, 1.0 });

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

							auto hit = Collision::PointLineCast(line1Start, line1End, line2Start, line2End);
							std::string message = "Expected " + (testCase.expectedResult.has_value() ? ""s : "no "s) + "collision between line from "s  + line1Start.ToString() + " to " + line1End.ToString()
								+ " and line from " + line2Start.ToString() + " to " + line2End.ToString();
							//Assert::AreEqual(hit, testCase.expectCollision);

							auto x = testCase.expectedResult.value() == hit.value();

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
			// TODO: Your test code here
		};

		

	};

}