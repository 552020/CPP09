#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

BitcoinExchange::BitcoinExchange(const std::string &DBFilename) : valid(false)
{
	if (checkFile(DBFilename) && parseDatabase(DBFilename) && validateDatabase())
	{
		valid = true;
	}
}

bool BitcoinExchange::isValid() const
{
	return valid;
}

bool BitcoinExchange::checkFile(const std::string &filename) const
{
	std::ifstream file(filename.c_str());
	return file.good();
}

bool BitcoinExchange::parseDatabase(const std::string &DBFilename)
{
	std::ifstream file(DBFilename.c_str());
	if (!file.is_open())
		std::cerr << "Error: could not open database file." << std::endl;
	return false;
}
