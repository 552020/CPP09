#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <regex>
#include <climits> // For INT_MAX and INT_MIN
#include <cfloat>  // For FLT_MAX and FLT_MIN

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos)
	{
		return "";
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

bool checkFileExistsAndReadable(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: The file \"" << filename << "\" does not exist or is not readable." << std::endl;
		return false;
	}
	return true;
}

bool isNumber(const std::string &str)
{
	if (str.empty())
		return false;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool isLeapYear(int year)
{
	if (year % 4 != 0)
		return false;
	if (year % 100 != 0)
		return true;
	if (year % 400 != 0)
		return false;
	return true;
}

bool checkDate(const std::string &date)
{
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	std::string yearStr = date.substr(0, 4);
	std::string monthStr = date.substr(5, 2);
	std::string dayStr = date.substr(8, 2);

	if (!isNumber(yearStr) || !isNumber(monthStr) || !isNumber(dayStr))
		return false;

	int year = std::atoi(yearStr.c_str());
	int month = std::atoi(monthStr.c_str());
	int day = std::atoi(dayStr.c_str());

	if (month < 1 || month > 12)
		return false;

	int daysInMonth = 31;
	if (month == 4 || month == 6 || month == 9 || month == 11)
		daysInMonth = 30;
	else if (month == 2)
	{
		daysInMonth = 28;
		if (isLeapYear(year))
			daysInMonth = 29;
	}

	if (day < 1 || day > daysInMonth)
		return false;

	return true;
}

bool checkExchangeRateValueDatabase(const std::string &exchangeRate)
{
	std::stringstream ss(exchangeRate);
	float num;
	if (!(ss >> num))
		return false;
	return num >= 0;
}

bool BitcoinExchange::validateDatabaseFile(const std::string &filename) const
{
	// TODO: eventually remove cause we already check for the existencce
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return false;
	}

	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		if (firstLine)
		{
			if (line != "date,exchange_rate")
			{
				std::cerr << "Error: invalid database file header." << std::endl;
				return false;
			}
		}
		firstLine = false;
		continue;
	}

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

BitcoinExchange::BitcoinExchange()
{
	std::string DBFilename = checkDatabase();
	if (DBFilename.empty())
	{
		std::cerr << "Error: no valid database file found." << std::endl;
		return;
	}
	else
	{
		if (!parseDatabase(DBFilename))
		{
			std::cerr << "Error: invalid database file." << std::endl;
			return;
		}
	}
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
	// Note: getline() is used to extracth the line
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

// bool isIntOrFloat(const std::string &str, float &value)
// {
// 	char *endPtr;
// 	value = std::strtof(str.c_str(), &endPtr);

// 	if (*endPtr != '\0')
// 		return false;

// 	return true;
// }

bool isIntOrFloat(const std::string &str, float &value)
{
	char *endPtr;

	// Try converting to a long first to check for integer overflow/underflow
	long intValue = std::strtol(str.c_str(), &endPtr, 10);

	if (*endPtr == '\0')
	{ // Successfully converted to an integer
		if (intValue > INT_MAX || intValue < INT_MIN)
		{
			std::cerr << "Error: integer overflow or underflow." << std::endl;
			return false;
		}
		value = static_cast<float>(intValue);
		return true;
	}

	// If not an integer, try converting to a float
	value = std::strtof(str.c_str(), &endPtr);

	if (*endPtr != '\0')
	{
		std::cerr << "Error: invalid numeric format." << std::endl;
		return false;
	}

	if (value > FLT_MAX || value < -FLT_MAX)
	{ // Checking for float overflow (underflow is considered 0 in many cases)
		std::cerr << "Error: floating-point overflow." << std::endl;
		return false;
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
	std::string line;
	bool firstLine = true;

	// Regular expression to match "string space | space string"
	std::regex lineFormatRegex("^\\s*\\S+\\s\\|\\s\\S+\\s*$");

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
		if (!std::regex_match(line, lineFormatRegex))
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
			// Find the closest date that is not after the given date
			std::map<std::string, float>::const_iterator it = _exchangeRates.lower_bound(date);
			// if (it != _exchangeRates.begin() && (it == _exchangeRates.end() || it->first != date))
			// {
			// 	--it; // Use the closest lower date if the exact date is not found
			// }

			// Check if the iterator points to the first element in the map
			if (it == _exchangeRates.begin())
			{
				// If the first element is greater than the searched date, no previous date exists
				if (it->first > date)
				{
					std::cerr << "Error: No valid previous exchange rate found for the date " << date << std::endl;
					// return;
					continue;
				}
			}

			// Check if the lower_bound points to a greater date or if it's at the end
			if (it == _exchangeRates.end() || it->first != date)
			{
				// If not at the beginning, move to the previous date
				if (it != _exchangeRates.begin())
				{
					--it;
				}
			}
			if (it != getExchangeRates().end())
			{
				float exchangeRate = it->second;
				float result = value * exchangeRate;
				std::cout << date << " => " << value << " = " << result << std::endl;
			}
		}
	}
}

// Copy constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _exchangeRates(other._exchangeRates)
{
}

// Destructor
BitcoinExchange::~BitcoinExchange()
{
}

// Assignment operator
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
