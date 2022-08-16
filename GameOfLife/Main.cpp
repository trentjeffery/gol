#include <memory>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>

#include <World.h>

// for easy testing
constexpr const char* s_inputFile = "../input.txt";

// tuning
constexpr const double s_desiredSecondsPerTick = 0.01; // limit tick rate. Set to 0 for no throttling

// command line args
bool s_printAliveCells = false;
bool s_printTickTime = false;

int main(int argc, char* argv[])
{
	// read command line args
	for (int i = 0; i < argc; ++i)
	{
		const char* arg = argv[i];

		s_printAliveCells = s_printAliveCells || (strcmp(arg, "-printAliveCells") == 0);
		s_printTickTime = s_printTickTime || (strcmp(arg, "-printTickTime") == 0);
	}

	// read input of input alive cell coordinates from file
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

	// create world
	std::unique_ptr<World> world = std::make_unique<World>(startingCells);

	// loop exit flag; currently always set false
	bool exit = false;
	
	int currentTick = 0;
	auto lastTickTime = std::chrono::steady_clock::now();;

	// run simulation
	while (!exit)
	{
		auto currentTickTime = std::chrono::steady_clock::now();; // todo this needs to be a float
		std::chrono::duration<double> timeSinceLastTick = currentTickTime - lastTickTime;

		if (timeSinceLastTick.count() < s_desiredSecondsPerTick)
		{
			continue;
		}

		if (s_printTickTime)
		{
			printf("Tick %d tick time: %f seconds\n", currentTick, timeSinceLastTick.count());
		}

		if (s_printAliveCells)
		{
			printf("Tick %d alive cells:\n", currentTick);
			for (const auto& cell : world->GetAliveCells())
			{
				printf("(%I64d, %I64d) ", cell.x, cell.y);
			}
			printf("\n");
		}
		else
		{
			printf("Tick %d. Number of alive cells:%zd\n", currentTick, world->GetAliveCells().size());
		}

		++currentTick;
		lastTickTime = currentTickTime;
		world->Update();
	}

	return 0;
}