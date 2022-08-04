#include "pch.h"
#include "CppUnitTest.h"

#include <algorithm>
#include <World.h>

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
		}

	};
}
