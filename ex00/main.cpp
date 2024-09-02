#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		std::cerr << "Error: could not open file. I mean ... you didn't provide any file!" << std::endl;
		std::cout << "Usage: ./btc <path/to/file>" << std::endl;
		return 1;
	}
	if (argc > 2)
	{
		std::cerr << "Error: too many arguments." << std::endl;
		std::cout << "Usage: ./btc <path/to/file>" << std::endl;
		return 1;
	}
	std::string inputFilename = argv[1];
	try
	{
		BitcoinExchange myExchange;
		myExchange.outputInputFile(argv[1]);
	}
	catch (const std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}