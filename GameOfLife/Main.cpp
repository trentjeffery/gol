#include <memory>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

#include <World.h>

// for easy testing
constexpr const char* s_inputFile = "C:/projects/gol/input.txt";
constexpr const float s_secondsPerTick = 0.016;

int main(int argc, char* argv[])
{
	// read input of alive cell coordinates
	Cells startingCells;
	std::ifstream inputFileStream(s_inputFile);
	for (char line[256]; inputFileStream.getline(line, 128);)
	{
		constexpr const char* delim = "(), ";
		std::string lineString(line);
		size_t xStartPos = lineString.find_first_not_of(delim);
		size_t xEndPos = lineString.find_first_of(delim, xStartPos);
		size_t yStartPos = lineString.find_first_not_of(delim, xEndPos);
		size_t yEndPos = lineString.find_first_of(delim, yStartPos);

		int64_t x = std::stoll(lineString.substr(xStartPos, (xEndPos - xStartPos)));
		int64_t y = std::stoll(lineString.substr(yStartPos, (yEndPos - yStartPos)));
		startingCells.emplace_back(x, y);
	}
	inputFileStream.close();

	// do any initialization
	std::unique_ptr<World> world = std::make_unique<World>(startingCells);

	bool exit = false;
	
	int tick = 0;
	std::time_t lastTickTime;
	time(&lastTickTime);
	// run simulation
	while (!exit)
	{
		std::time_t currentTime; // todo this needs to be a float
		time(&currentTime);

		if ((currentTime - lastTickTime) < s_secondsPerTick)
		{
			continue;
		}

		printf("Tick %d alive cells:\n", tick);
		for (const auto& cell : world->GetAliveCells())
		{
			printf("(%I64d, %I64d) ", cell.x, cell.y);
		}
		printf("\n");

		time(&lastTickTime);
		++tick;

		world->Update();

	}

	// do any cleanup

	return 0;
}