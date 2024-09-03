#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cctype>
#include "PmergeMe.hpp"
#include "utils.hpp"
#include "group_iterator_tests.hpp"

void runTests()
{
	std::cout << "Running tests" << std::endl;
	PmergeMe merger;
	// Test testVectorSort
	int arr[] = {5, 4, 3, 2, 1, 8, 7, 10, 9, 6};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));
	// merger.testVectorSort(vec, true);

	// Test multipleTestVectorSort
	// merger.multipleTestVectorSort(10, 100, 1000, false);

	// Test testDequeSort
	int arr2[] = {5, 4, 3, 2, 1, 8, 7, 10, 9, 6};
	std::deque<int> deq(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]));
	// merger.testDequeSort(deq, true);

	// Test multipleTestDequeSort
	merger.multipleTestDequeSort(10, 100, 1000, false);
}

int main(int argc, char **argv)
{
	std::string input(argv[1]);
	if (argc == 2 && input == "test")
	{
		runTests();
		return 0;
	}
	std::vector<int> numbers;
	if (!checkInputAndFillNumbersVec(argc, argv, numbers))
		return 1;

	PmergeMe pmergeMe(numbers);

	// pmergeMe.sortVec(true);
	// pmergeMe.sortDeque(true);
	pmergeMe.sortVecAndDeque();

	return 0;
}
