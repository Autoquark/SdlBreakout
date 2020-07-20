#include "stdafx.h"

#include "CircleF.h"
#include "Collision.h"
#include "ToString.h"
#include "FloatCompare.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CircleLineCastTests)
	{
		struct TestCase
		{
			CircleF circle;
			Vector2F lineStart;
			Vector2F lineEnd;
			Vector2F motion;
		};

		void RunTestCase(const TestCase& testCase)
		{
			//auto result = Collision::PointCircleCast();
		}

	public:
		TEST_METHOD(HitHorizontalLine)
		{
			TestCase testCase;
			testCase.lineStart = Vector2F(-2, 2);
			testCase.lineEnd = Vector2F(2, 2);
			testCase.circle = CircleF(1, 5, 1);
			testCase.motion = Vector2F(0, -10);

			RunTestCase(testCase);
		}
	};
}