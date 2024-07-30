#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
  public:
	BitcoinExchange(const std::string &DBFilename);
	bool isValid() const;

  private:
	bool parseDatabase(const std::string &DBFilename);
	bool validateDatabase() const;
	bool checkFile(const std::string &filename) const;
	bool checkDate(const std::string &date) const;
	bool checkValue(const std::string *value, bool isExchangeRage) const;

	std::map<std::string, float> exchangeRates;
	bool valid;
};

#endif