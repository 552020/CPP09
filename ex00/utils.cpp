#include "utils.hpp"

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
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

	// Checking for float overflow (underflow is considered 0 in many cases)
	if (value > FLT_MAX || value < -FLT_MAX)
	{
		std::cerr << "Error: floating-point overflow." << std::endl;
		return false;
	}

	return true;
}

bool isValidFormat(const std::string &line)
{
	size_t separatorPos = line.find('|');
	if (separatorPos == std::string::npos)
		return false; // No '|' found
	if (separatorPos == 0 || separatorPos == line.length() - 1)
		return false; // '|' at start or end

	// Ensure there's exactly one space before and after '|'
	if (line[separatorPos - 1] != ' ' || line[separatorPos + 1] != ' ')
		return false;

	// Check that there's text (non-space) before and after the separator
	if (line.find_first_not_of(' ') >= separatorPos || line.find_last_not_of(' ') <= separatorPos)
		return false;

	return true;
}
