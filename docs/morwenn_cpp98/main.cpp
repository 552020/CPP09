#include <iostream>
#include <vector>
#include "GroupIterator.hpp"

int main()
{
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Test the default constructor
	GroupIterator<std::vector<int>::iterator> defaultGroupIt;
	std::cout << "Default GroupIterator created. Size: " << defaultGroupIt.size() << std::endl;

	// Test the parameterized constructor
	GroupIterator<std::vector<int>::iterator> groupIt(vec.begin(), 3);
	std::cout << "Parameterized GroupIterator created. Starting base: " << (groupIt.base() - vec.begin()) << std::endl;

	// Iterate through the groups
	while (groupIt.base() != vec.end())
	{
		// Check if there is enough room left for a full group
		if (static_cast<std::size_t>(std::distance(groupIt.base(), vec.end())) < groupIt.size())
		{
			std::cout << "No more full groups left to process." << std::endl;
			break;
		}

		std::cout << "Group last element: " << *groupIt << std::endl;

		// Move to the next group
		++groupIt;
	}

	std::cout << "Test iter_swap" << std::endl;

	// Initialize a vector with some test data
	int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int arr2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	std::vector<int> vec1(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]));
	std::vector<int> vec2(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]));

	// Create two GroupIterator instances for different parts of the vectors
	GroupIterator<std::vector<int>::iterator> groupIt1(vec1.begin(), 3);
	GroupIterator<std::vector<int>::iterator> groupIt2(vec2.begin(), 3);

	// Print initial state of the vectors
	std::cout << "Before iter_swap:" << std::endl;
	std::cout << "vec1: ";
	for (std::vector<int>::iterator it = vec1.begin(); it != vec1.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << "vec2: ";
	for (std::vector<int>::iterator it = vec2.begin(); it != vec2.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	// Perform the iter_swap
	iter_swap(groupIt1, groupIt2);

	// Print the state of the vectors after swapping
	std::cout << "After iter_swap:" << std::endl;
	std::cout << "vec1: ";
	for (std::vector<int>::iterator it = vec1.begin(); it != vec1.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << "vec2: ";
	for (std::vector<int>::iterator it = vec2.begin(); it != vec2.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	return 0;
}
