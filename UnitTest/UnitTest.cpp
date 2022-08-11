#include "pch.h"
#include "CppUnitTest.h"

#include <algorithm>
#include <World.h>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		TEST_METHOD(SampleInputUpdate)
		{
			Cells startingCells;
			startingCells.emplace_back(0, 1);
			startingCells.emplace_back(1, 2);
			startingCells.emplace_back(2, 0);
			startingCells.emplace_back(2, 1);
			startingCells.emplace_back(2, 2);
			startingCells.emplace_back(-2000000000000, -2000000000000);
			startingCells.emplace_back(-2000000000001, -2000000000001);

			World world(startingCells);
			const auto& aliveCells = world.GetAliveCells();
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(1, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2000000000000, -2000000000000)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2000000000001, -2000000000001)) != aliveCells.end());

			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(1, 0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(1, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(3, 1)) != aliveCells.end());
			
			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, 1)) != aliveCells.end());
			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 0)) != aliveCells.end());
			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2000000000000, -2000000000000)) != aliveCells.end());
			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2000000000001, -2000000000001)) != aliveCells.end());

			Assert::IsTrue(aliveCells.size() == 5);

			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(3, 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(1, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2, 2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(3, 2)) != aliveCells.end());

			Assert::IsTrue(aliveCells.size() == 5);
		}

		TEST_METHOD(BoundariesXMin)
		{
			Cells startingCells;
			startingCells.emplace_back(INT64_MIN, -1);
			startingCells.emplace_back(INT64_MIN, 0);
			startingCells.emplace_back(INT64_MIN, 1);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();
			
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN + 1, 0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN, 0)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(2));
			world.Update();

			Assert::AreEqual(aliveCells.size(), size_t(0));
		}

		TEST_METHOD(BoundariesXMax)
		{
			Cells startingCells;
			startingCells.emplace_back(INT64_MAX, -1);
			startingCells.emplace_back(INT64_MAX, 0);
			startingCells.emplace_back(INT64_MAX, 1);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX - 1, 0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX, 0)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(2));
			world.Update();

			Assert::AreEqual(aliveCells.size(), size_t(0));
		}

		TEST_METHOD(BoundariesYMin)
		{
			Cells startingCells;
			startingCells.emplace_back(-1, INT64_MIN);
			startingCells.emplace_back(0, INT64_MIN);
			startingCells.emplace_back(1, INT64_MIN);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, INT64_MIN + 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, INT64_MIN)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(2));
			world.Update();

			Assert::AreEqual(aliveCells.size(), size_t(0));
		}

		TEST_METHOD(BoundariesYMax)
		{
			Cells startingCells;
			startingCells.emplace_back(-1, INT64_MAX);
			startingCells.emplace_back(0, INT64_MAX);
			startingCells.emplace_back(1, INT64_MAX);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, INT64_MAX - 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, INT64_MAX)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(2));
			world.Update();

			Assert::AreEqual(aliveCells.size(), size_t(0));
		}

		TEST_METHOD(MaxCorner)
		{
			Cells startingCells;
			startingCells.emplace_back(INT64_MAX, INT64_MAX);
			startingCells.emplace_back(INT64_MAX-1, INT64_MAX);
			startingCells.emplace_back(INT64_MAX, INT64_MAX-1);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX, INT64_MAX)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX-1, INT64_MAX)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX, INT64_MAX-1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX-1, INT64_MAX-1)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(4));
			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX, INT64_MAX)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX - 1, INT64_MAX)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX, INT64_MAX - 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MAX - 1, INT64_MAX - 1)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(4));
		}

		TEST_METHOD(MinCorner)
		{
			Cells startingCells;
			startingCells.emplace_back(INT64_MIN, INT64_MIN);
			startingCells.emplace_back(INT64_MIN + 1, INT64_MIN);
			startingCells.emplace_back(INT64_MIN, INT64_MIN + 1);

			World world(startingCells);
			world.Update();
			const auto& aliveCells = world.GetAliveCells();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN, INT64_MIN)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN + 1, INT64_MIN)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN, INT64_MIN + 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN + 1, INT64_MIN + 1)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(4));
			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN, INT64_MIN)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN + 1, INT64_MIN)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN, INT64_MIN + 1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(INT64_MIN + 1, INT64_MIN + 1)) != aliveCells.end());
			Assert::AreEqual(aliveCells.size(), size_t(4));
		}

		TEST_METHOD(NegativeNumberedCellsCanLive)
		{
			Cells startingCells;
			startingCells.emplace_back(-0, -1);
			startingCells.emplace_back(-1, -2);
			startingCells.emplace_back(-2, -0);
			startingCells.emplace_back(-2, -1);
			startingCells.emplace_back(-2, -2);
			startingCells.emplace_back(2000000000000, 2000000000000);
			startingCells.emplace_back(2000000000001, 2000000000001);

			World world(startingCells);
			const auto& aliveCells = world.GetAliveCells();
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(0, -1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-1, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2000000000000, 2000000000000)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(2000000000001, 2000000000001)) != aliveCells.end());

			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-1, -0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-1, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-3, -1)) != aliveCells.end());

			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-0, -1)) != aliveCells.end());
			Assert::IsFalse(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -0)) != aliveCells.end());

			Assert::IsTrue(aliveCells.size() == 5);

			world.Update();

			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -0)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-3, -1)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-1, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-2, -2)) != aliveCells.end());
			Assert::IsTrue(std::find(aliveCells.begin(), aliveCells.end(), Coordinate(-3, -2)) != aliveCells.end());

			Assert::IsTrue(aliveCells.size() == 5);
		}
	};
}
