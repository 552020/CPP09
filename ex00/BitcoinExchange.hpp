#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>		// For std::map to store exchange rates by date
#include <string>	// For std::string
#include <dirent.h> // For DIR, dirent, opendir, readdir, closedir

class BitcoinExchange
{
  public:
	BitcoinExchange();										  // Default constructor
	BitcoinExchange(const BitcoinExchange &other);			  // Copy constructor
	BitcoinExchange &operator=(const BitcoinExchange &other); // Assignment operator
	~BitcoinExchange();										  // Destructor

	const std::map<std::string, float> &getExchangeRates() const;
	void outputInputFile(const std::string &inputFilename);

  private:
	std::string checkDatabase(const std::string &databaseFilename = "data.csv") const;
	bool validateDatabaseFile(const std::string &filename) const;

	bool parseDatabase(const std::string &DBFilename);

	std::map<std::string, float> _exchangeRates;
};

#endif