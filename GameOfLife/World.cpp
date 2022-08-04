#include "World.h"

#include <algorithm>
#include <limits>

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
		}
	}

	_ASSERT(result.size() <= 8);
	_ASSERT(result.size() >= 3);
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
