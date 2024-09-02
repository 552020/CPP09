#include <iostream> // For std::cout, std::cerr, std::endl
#include <fstream>	// For std::ifstream
#include <sstream>	// For std::stringstream
#include <map>		// For std::map

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		std::cerr << "Error: could not open file. I mean ... you didn't provide any file!" << std::endl;
		return 1;
	}
	if (argc > 2)
	{
		std::cerr << "Error: too many arguments." << std::endl;
		return 1;
	}
	std::string inputFilename = argv[1];
	BitcoinExchange myExchange;
	myExchange.outputInputFile(inputFilename);

	return 0;
}