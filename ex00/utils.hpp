#pragma once

#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cctype>
#include <iostream>
#include <climits> // For INT_MAX and INT_MIN
#include <cfloat>  // For FLT_MAX and FLT_MIN

std::string trim(const std::string &str);

bool isNumber(const std::string &str);

bool isLeapYear(int year);

bool checkDate(const std::string &date);
bool checkExchangeRateValueDatabase(const std::string &exchangeRate);
bool checkFileExistsAndReadable(const std::string &filename);
bool isIntOrFloat(const std::string &str, float &value);
bool isValidFormat(const std::string &line);
