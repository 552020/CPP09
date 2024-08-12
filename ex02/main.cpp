#include <string>
#include <iostream>
#include <cstdlib> // for strtol
#include <vector>
#include <sstream>
#include <cctype> // for
#include "PmergeMe.hpp"
#include "utils.hpp"
#include "group_iterator_tests.hpp"

bool isNumber(const std::string &input)
{
	for (std::string::const_iterator it = input.begin(); it != input.end(); ++it)
	{
		if (!std::isdigit(*it))
			return false;
	}
	return true;
}

bool checkInputAndFillNumersVec(int argc, char **argv, std::vector<int> &numbers)
{
	// std::cout << "Passed arguments: " << argc - 1 << std::endl;
	std::string input;
	if (argc < 2)
	{
		std::cout << "Usage " << argv[0] << " <some numbers>. Only positive numbers. Sign is also not tolerated!"
				  << std::endl;
		return false;
	}
	else if (argc == 2)
		input = argv[1];
	// If the user input is passed not as a single string, but as a list of arguments (e.g. "1" "2" "3")
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			if (i > 1)
				input += " ";
			input += argv[i];
		}
	}

	std::stringstream ss(input);
	std::string token;
	while (ss >> token)
	{
		if (!isNumber(token))
		{
			std::cout << "Invalid input. Only positive numbers are allowed, with no signs. Mixed inputs like '\"1 2\" "
						 "2' are also not tolerated!"
					  << std::endl;

			return false;
		}
		long num = std::strtol(token.c_str(), NULL, 10);
		if (num > INT_MAX || num <= 0)
		{
			std::cout << "Invalid input. Number exceeds maximum allowed value (2147483647) or is non-positive."
					  << std::endl;

			return false;
		}
		numbers.push_back(static_cast<int>(num));
	}
	if (numbers.empty())
	{
		std::cout << "No numbers were provided." << std::endl;
		return false;
	}
	return true;
}

int main(int argc, char **argv)
{
	std::vector<int> numbers;
	if (!checkInputAndFillNumersVec(argc, argv, numbers))
		return 1;

	PmergeMe merger(numbers);
	// TODO I need also a default constructor for when we don't pass any arugment
	// TODO test all the functions.
	return 0;
}
