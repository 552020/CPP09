#include <string>
#include <iostream>
#include <cstdlib> // for strtol
#include <vector>
#include <sstream>
#include <cctype> // for
#include "PmergeMe.hpp"
#include "utils.hpp"
#include "group_iterator_tests.hpp"

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
