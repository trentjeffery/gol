#pragma once

#include <vector>

// used to represent a cell coordinate in the game of life
struct Coordinate;
struct Coordinate
{
	int64_t x;
	int64_t y;

public:
	Coordinate(int64_t xIndex, int64_t yIndex) : x(xIndex), y(yIndex) {};
	std::vector<Coordinate> GetNeighbours() const;
	bool operator==(const Coordinate& rhs) const = default;
};

struct PotentialAliveCell
{
	const Coordinate coordinate;
	uint16_t neighbourCount = 0;
	bool alivePreviousTick;

public:
	PotentialAliveCell(const Coordinate& coordinate, bool alivePreviousTick, uint16_t neighbourCount = 0)
		: coordinate(coordinate)
		, neighbourCount(neighbourCount)
		, alivePreviousTick(alivePreviousTick)
	{};
};

static bool operator==(const Coordinate& lhs, const PotentialAliveCell& rhs)
{
	return lhs == rhs.coordinate;
}

typedef std::vector<Coordinate> Cells;

class World
{
public:
	World() {};
	World(const Cells& startingAliveCells)
		: m_aliveCells(startingAliveCells)
	{}
	World(Cells&& startingAliveCells) 
	: m_aliveCells() 
	{}

	void Update();

	const Cells& GetAliveCells() { return m_aliveCells; }

private:
	// list of alive cells in last completed tick
	std::vector<Coordinate> m_aliveCells;
};

