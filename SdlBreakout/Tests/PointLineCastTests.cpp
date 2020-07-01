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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace Tests
{	
	TEST_CLASS(PointLineCastTests)
	{
		struct TestCase
		{
			/*TestCase() : name(L""), pointStart(), pointEnd(1, 1), lineStart(), lineEnd(), expectedResult()
			{
				std::cout << "test";
			}*/

			std::wstring name;
			// = L"untitled";
			Vector2 pointStart;
			Vector2 pointEnd;
			Vector2 lineStart;
			Vector2 lineEnd;

			std::optional<Contact> expectedResult;
		};

	public:
		
		TEST_METHOD(PointLineCast)
		{
			std::vector<TestCase> testCases;
			TestCase testCase;// = TestCase();

			testCase.name = L"Crossing diagonal lines";
			testCase.pointStart = { 0.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 2.0, 0.0 };
			testCase.lineEnd = { 0.0, 2.0 };

			testCase.expectedResult = Contact(Vector2(-1, -1).Normalised(), Vector2{ 1.0, 1.0 });
			testCases.emplace_back(testCase);

			// Non-crossing diagonal lines
			testCase = TestCase();
			testCase.pointStart = { 0.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 3.0, 2.0 };
			testCase.lineEnd = { 5.0, 0.0 };

			testCase.expectedResult = std::nullopt;
			testCases.emplace_back(testCase);

			// Non-overlapping vertical lines
			testCase = TestCase();
			testCase.pointStart = { 2.0, 0.0 };
			testCase.pointEnd = { 2.0, 2.0 };

			testCase.lineStart = { 3.0, 1.0 };
			testCase.lineEnd = { 3.0, 3.0 };

			testCase.expectedResult = std::nullopt;
			testCases.emplace_back(testCase);

			// Passing beneath vertical line
			testCase = TestCase();
			testCase.pointStart = Vector2(-2, 0);
			testCase.pointEnd = Vector2(2, 4);

			testCase.lineStart = Vector2(0, 0);
			testCase.lineEnd = Vector2(3, -3);

			testCase.expectedResult = std::nullopt;
			testCases.emplace_back(testCase);

			// Hitting vertical line
			testCase = TestCase();
			testCase.pointStart = Vector2(-2, 2);
			testCase.pointEnd = Vector2(2, 2);

			testCase.lineStart = Vector2(0, 0);
			testCase.lineEnd = Vector2(0, 3);

			testCase.expectedResult = Contact(Vector2(-1, 0).Normalised(), Vector2(0, 2));
			testCases.emplace_back(testCase);

			// Overlapping vertical lines
			// Don't know what if anything in particular is a useful return value here
			/*i++;
			testCases[i].pointStart = { 2.0, 0.0 };
			testCases[i].pointEnd = { 2.0, 2.0 };

			testCases[i].lineStart = { 2.0, 1.0 };
			testCases[i].lineEnd = { 2.0, 3.0 };

			testCases[i].expectedResult = Contact(Vector2{ 0, 0 }, Vector2 { 2.0, 1.0 });*/

			for (auto testCase : testCases)
			{
				for (auto swapPointAndLine : { false, true})
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
							std::wstring message = L"Case "s + testCase.name + L" Failed. Expected "s + (testCase.expectedResult.has_value() ? L""s : L"no "s) + L"collision between line from "s
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

								// Don't check the normal if the line & point are swapped as it's not trivially calculable from the normal in the original case
								if (!swapPointAndLine)
								{
									Assert::AreEqual(swapPointEnds ? -expected.normal : expected.normal, actual.normal, message.c_str());
								}

							}
						}
					}
				}
			}
		}
	};

}