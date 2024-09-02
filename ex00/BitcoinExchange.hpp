#pragma once

#include <map>
#include <string>
#include <dirent.h>
#include <cstdlib>
#include "utils.hpp"

class BitcoinExchange
{
  public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	const std::map<std::string, float> &getExchangeRates() const;
	void outputInputFile(const std::string &inputFilename);

  private:
	std::string checkDatabase(const std::string &databaseFilename = "data.csv") const;
	bool validateDatabaseFile(const std::string &filename) const;

	bool parseDatabase(const std::string &DBFilename);

	std::map<std::string, float> _exchangeRates;
};
