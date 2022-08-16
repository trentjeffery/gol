#include "World.h"

#include <algorithm>
#include <limits>

#include <cassert>

std::vector<Coordinate> Coordinate::GetNeighbours() const
{
	std::vector<Coordinate> result;
	result.reserve(8); // max 8 cells, and most cells have 8 neighbours
	
	int64_t lowerX = x > INT64_MIN ? x - 1 : x;
	int64_t upperX = x < INT64_MAX ? x + 1 : x;
	int64_t lowerY = y > INT64_MIN ? y - 1 : y;
	int64_t upperY = y < INT64_MAX ? y + 1 : y;

	for (int64_t i = lowerX; i <= upperX; ++i)
	{
		for (int64_t j = lowerY; j <= upperY; ++j)
		{
			// don't include self coordinate
			if (i != x || j != y)
			{
				result.emplace_back(i, j);
			}

			// if we do the loop update increment while j is INT64_MAX it will overflow to negative and we'll infinitely loop
			// don't need to check for -INT64_MAX because we don't decrement the loop variable
			if (j == INT64_MAX)
			{
				break;
			}
		}

		// if we do the loop update increment while x is INT64_MAX it will overflow to negative and we'll infinitely loop
		// don't need to check for -INT64_MAX because we don't decrement the loop variable
		if (i == INT64_MAX)
		{
			break;
		}
	}
	
	assert(result.size() <= 8);
	assert(result.size() >= 3);

	return result;
}

void World::Update()
{
	// update the simulation
	std::vector<PotentialAliveCell> potentialAliveCells;
	
	// for each cell alive previous tick, mark as such and increment neighbour count of each of our neighbours
	for (const auto& aliveCell : m_aliveCells)
	{
		const auto& foundCell = std::find(potentialAliveCells.begin(), potentialAliveCells.end(), aliveCell);
		if (foundCell == potentialAliveCells.end())
		{
			potentialAliveCells.emplace_back(aliveCell, true, 0);
		}
		else
		{
			// if we were added to the list by another cell, it thought we were newly alive
			foundCell->alivePreviousTick = true;
		}

		const Cells neighbours = aliveCell.GetNeighbours();
		for (const auto& neighbour : neighbours)
		{
			// time complexity and possible speed improvement notes:
			// let n be size of m_aliveCells. Potential alive cells size is O(n) (it will be at most 9*n in the case no alive cells are neighbours)
			// current approach does O(n) searches. We're currently using an unsorted vector so that's O(n) linear searches for a total time of O(n^2)
			// We could use a sorted container or different container to get O(log(n)) searches, which would get our update down to O(nlog(n)) time
			
			// would it be faster to just make a huge long list of all the potential alive cells, allowing duplicates, and then folding them all together (instead of doing a bunch of finds)?
			// that approach would give us linear time to create all entries. 
			// How much time to combine all entries? We need to pre-sort them or effectively sort them as we go to get same-cell entries together, total for merging cells should be O(nlog(n)) 
			//  - if we pre-sort, sort takes O(nlog(n)) and merge pass takes O(n), for total of O(nlog(n))
			//  - if we don't sort, we need to do something like a merge/insertion for each element as we combine, which I belive would take O(log(n)) per element again giving us O(nlog(n))

			const auto& foundNeighbour = std::find(potentialAliveCells.begin(), potentialAliveCells.end(), neighbour);
			if (foundNeighbour == potentialAliveCells.end())
			{
				// assume this is a new cell this tick. If it was alive last tick, it hasn't processed yet and will update its 
				// alivePreviousTick flag when it does
				potentialAliveCells.emplace_back(neighbour, false, 1);
			}
			else
			{
				foundNeighbour->neighbourCount += 1;
			}
		}
	}

	m_aliveCells.clear();
	for (const auto& potentialAliveCell : potentialAliveCells)
	{
		if (potentialAliveCell.neighbourCount == 3 || (potentialAliveCell.alivePreviousTick && potentialAliveCell.neighbourCount == 2))
		{
			m_aliveCells.emplace_back(potentialAliveCell.coordinate);
		}
	}
}