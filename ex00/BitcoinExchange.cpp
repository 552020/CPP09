#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

BitcoinExchange::BitcoinExchange()
{
	std::string DBFilename = checkDatabase();
	if (DBFilename.empty())
		throw std::runtime_error("Error: no valid database file found.");
	else
	{
		if (!parseDatabase(DBFilename))
			throw std::runtime_error("Error: could not parse database file.");
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _exchangeRates(other._exchangeRates)
{
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		_exchangeRates = other._exchangeRates;
	}
	return *this;
}

const std::map<std::string, float> &BitcoinExchange::getExchangeRates() const
{
	return _exchangeRates;
}

bool BitcoinExchange::validateDatabaseFile(const std::string &filename) const
{
	// TODO: eventually remove cause we already check for the existence
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return false;
	}

	std::string line;
	if (std::getline(file, line))
	{
		if (line != "date,exchange_rate")
		{
			std::cerr << "Error: invalid database file header." << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "Error: empty database file." << std::endl;
		return false;
	}

	while (std::getline(file, line))
	{

		std::stringstream ss(line);
		std::string date;
		std::string rateStr;

		if (std::getline(ss, date, ',') && std::getline(ss, rateStr))
		{
			date = trim(date);
			rateStr = trim(rateStr);
			if (!checkDate(date) || !checkExchangeRateValueDatabase(rateStr))
			{
				std::cerr << "Warning: invalid data format in database file on line: " << line << std::endl;
				return false;
			}
		}
	}
	return true;
}

std::string BitcoinExchange::checkDatabase(const std::string &databaseFilename) const
{
	if (checkFileExistsAndReadable(databaseFilename) && validateDatabaseFile(databaseFilename))
	{
		return databaseFilename;
	}
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(".")) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string filename = ent->d_name;
			if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".csv")
			{
				if (checkFileExistsAndReadable(filename) && validateDatabaseFile(filename))
				{
					closedir(dir);
					return filename;
				}
			}
		}
		closedir(dir);
	}
	return "";
}

bool BitcoinExchange::parseDatabase(const std::string &DBFilename)

{
	std::ifstream file(DBFilename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return false;
	}

	std::string line;
	bool firstLine = true;
	// Note: getline() is used to extract the line
	while (std::getline(file, line))
	{
		if (firstLine)
		{
			if (line != "date,exchange_rate")
			{
				std::cerr << "Error: invalid database format." << std::endl;
				return false;
			}
			firstLine = false;
			continue;
		}
		std::stringstream ss(line);
		std::string date;
		float rate;
		// Note: getline is used to extract the date and rate
		if (std::getline(ss, date, ',') && ss >> rate)
		{
			_exchangeRates[date] = rate;
		}
		else
		{
			std::cerr << "Error: invalid database format." << std::endl;
			return false;
		}
	}
	return true;
}

void BitcoinExchange::outputInputFile(const std::string &inputFilename)
{
	std::ifstream file(inputFilename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open input file." << std::endl;
		return;
	}

	// Check if the map is empty first // We do this check right before starting to interact with the map
	if (_exchangeRates.empty())
	{
		std::cerr << "Error: No exchange rates available." << std::endl;
		return;
	}

	std::string line;
	bool firstLine = true;

	while (std::getline(file, line))
	{
		if (firstLine)
		{
			if (line != "date | value")
			{
				std::cerr << "Error: The header of the input file it doesn't look like as it should. But who cars. "
							 "Let's continue!"
						  << std::endl;
			}
			firstLine = false;
			continue;
		}
		if (!isValidFormat(line))
		{
			std::cerr << "Error: invalid format in input file. Expected 'string space | space string'." << std::endl;
			continue;
		}

		std::stringstream ss(line);
		std::string date;
		std::string valueStr;
		if (std::getline(ss, date, '|') && std::getline(ss, valueStr))
		{
			date = trim(date);
			valueStr = trim(valueStr);

			float value;
			if (!isIntOrFloat(valueStr, value))
			{
				std::cerr << "Error: bad input => " << valueStr << std::endl;
				continue;
			}
			if (!checkDate(date))
			{
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}
			if (value <= 0)
			{
				std::cerr << "Error: not a positive number." << std::endl;
				continue;
			}
			if (value > 1000)
			{
				std::cerr << "Error: too large value." << std::endl;
				continue;
			}

			// Find the closest date that is not after the given date, the lower_bound() function is used to find the
			// first element that is not less than the given date (means greater or equal)
			std::map<std::string, float>::const_iterator it = _exchangeRates.lower_bound(date);

			// Check if the iterator points to the first element in the map
			if (it == _exchangeRates.begin())
			{
				// If the first element is greater than the searched date, no previous date exists
				if (it->first > date)
				{
					std::cerr << "Error: No valid previous exchange rate found for the date " << date << std::endl;
					continue;
				}
			}

			// Check if the lower_bound points to a greater date or if it's at the end
			if (it == _exchangeRates.end() || it->first != date)
			{
				// If not at the beginning, move to the previous date
				if (it != _exchangeRates.begin())
					--it;
			}
			float exchangeRate = it->second;
			float result = value * exchangeRate;
			std::cout << date << " => " << value << " = " << result << std::endl;
		}
	}
}
