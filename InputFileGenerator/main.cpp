#include <fstream>

const char* s_outputFile = "C:/projects/gol/output.txt";

void AddCoordinate(std::ostream& ostream, int64_t x, int64_t y)
{
	ostream << "(" << x << ", " << y << ")\n";
}

int main(int argc, char* argv[])
{
	std::ofstream outputFileStream(s_outputFile);

	for (int64_t i = 0; i < 1000; i+=3)
	{
		int64_t i3 = 3 * i;
		AddCoordinate(outputFileStream, i3, i3+1);
		AddCoordinate(outputFileStream, i3+1, i3+2);
		AddCoordinate(outputFileStream, i3+2, i3);
		AddCoordinate(outputFileStream, i3+2, i3+1);
		AddCoordinate(outputFileStream, i3+2, i3+2);
	}

	return 0;
}