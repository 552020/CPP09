#include <iostream>
#include <vector>
#include "GroupIterator.hpp"

#include <iostream>
#include <vector>
#include "GroupIterator.hpp" // Include your GroupIterator header

#include <iostream>
#include <vector>
#include "GroupIterator.hpp" // Include your GroupIterator header

// Function to test the dereference operator (operator*)
void testDereferenceOperator()
{
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Start groupIt from the beginning with a group size of 3
	GroupIterator<std::vector<int>::iterator> groupIt(vec.begin(), 3);

	std::cout << "Test dereference operator (operator*)\n";

	// Iterate through the groups and check the last element of each group
	for (std::size_t i = 0; i < vec.size() / groupIt.size(); ++i)
	{
		int actualValue = *groupIt;
		int expectedValue = 3 + (i * 3);

		std::cout << "Group starting at index " << (groupIt.base() - vec.begin()) << " last element: " << actualValue
				  << " (Expected: " << expectedValue << ")\n";

		++groupIt;
	}

	// Edge case: Test operator* when iterator is out of bounds
	try
	{
		GroupIterator<std::vector<int>::iterator> outOfBoundsIt = groupIt + 1; // This should move it out of bounds
		int actualValue = *outOfBoundsIt;
		std::cout << "Accessing out of bounds: " << actualValue
				  << " (Expected: out_of_range exception or invalid value)\n";
	}
	catch (const std::out_of_range &e)
	{
		std::cout << "Caught out_of_range exception as expected: " << e.what() << std::endl;
	}
}

// Function to test arithmetic operators
void testArithmeticOperators()
{
	std::cout << "\nTest arithmetic operators" << std::endl;
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Start groupIt from the element 6
	GroupIterator<std::vector<int>::iterator> groupIt(vec.begin() + 1, 3); // Points to {6, 7, 8}
	std::cout << "Parameterized GroupIterator created. Starting base: " << (groupIt.base() - vec.begin()) << std::endl;
	std::cout << "Initial group last element: " << *groupIt << " (Expected: 4)" << std::endl;

	// Test addition operator (iterator + numGroups)
	GroupIterator<std::vector<int>::iterator> groupIt2 = groupIt + 1; // Move forward by 1 group
	std::cout << "After groupIt + 1 group, last element: " << *groupIt2 << " (Expected: 7)" << std::endl;
	std::cout << "After groupIt + 1 group, base pointer: " << (groupIt2.base() - vec.begin()) << std::endl;

	// Test addition operator (numGroups + iterator)
	GroupIterator<std::vector<int>::iterator> groupIt3 = 1 + groupIt; // Move forward by 1 group
	std::cout << "After 1 + groupIt, last element: " << *groupIt3 << " (Expected: 7)" << std::endl;
	std::cout << "After 1 + groupIt, base pointer: " << (groupIt3.base() - vec.begin()) << std::endl;

	// Test subtraction operator (iterator - numGroups)
	GroupIterator<std::vector<int>::iterator> groupIt4 = groupIt2 - 1; // Move backward by 1 group
	std::cout << "After groupIt2 - 1 group, last element: " << *groupIt4 << " (Expected: 4)" << std::endl;
	std::cout << "After groupIt2 - 1 group, base pointer: " << (groupIt4.base() - vec.begin()) << std::endl;

	// Test difference operator (iterator1 - iterator2)
	typedef GroupIterator<std::vector<int>::iterator>::difference_type DifferenceType;
	DifferenceType diff = groupIt2 - groupIt;
	std::cout << "Difference between (groupIt2 and groupIt): " << diff << " groups (Expected: 1)" << std::endl;

	// Edge case: Move beyond the bounds of the vector
	GroupIterator<std::vector<int>::iterator> groupIt5 = groupIt + 3; // Move forward by 3 groups
	if (groupIt5.base() >= vec.end())
	{
		std::cout << "groupIt5 is out of bounds (as expected)." << std::endl;
	}
	else
	{
		std::cout << "groupIt5 last element: " << *groupIt5 << " (Unexpected result)" << std::endl;
	}
}

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

	// Reinitialize groupIt before testing the comparison operators
	groupIt = GroupIterator<std::vector<int>::iterator>(vec.begin(), 3);
	// Test the comparison operators
	std::cout << "\nTest comparison operators" << std::endl;

	GroupIterator<std::vector<int>::iterator> groupIt3(vec.begin(), 3);
	GroupIterator<std::vector<int>::iterator> groupIt4(vec.begin() + 3, 3);

	// Debug information
	std::cout << "groupIt base: " << (groupIt.base() - vec.begin()) << std::endl;
	std::cout << "groupIt3 base: " << (groupIt3.base() - vec.begin()) << std::endl;
	std::cout << "groupIt4 base: " << (groupIt4.base() - vec.begin()) << std::endl;

	// Print the internal details of the iterators
	std::cout << "groupIt: _it = " << &(*groupIt.base()) << ", _size = " << groupIt.size() << std::endl;
	std::cout << "groupIt3: _it = " << &(*groupIt3.base()) << ", _size = " << groupIt3.size() << std::endl;
	std::cout << "groupIt4: _it = " << &(*groupIt4.base()) << ", _size = " << groupIt4.size() << std::endl;

	// Test equality operator
	if (groupIt == groupIt3)
	{
		std::cout << "groupIt is equal to groupIt3 (pointing to the same position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt is NOT equal to groupIt3 (they should be equal)." << std::endl;
	}

	// Test inequality operator
	if (groupIt != groupIt4)
	{
		std::cout << "groupIt is NOT equal to groupIt4 (pointing to different positions)." << std::endl;
	}
	else
	{
		std::cout << "groupIt is equal to groupIt4 (they should be different)." << std::endl;
	}

	// Move groupIt to the next group and compare again
	++groupIt;

	if (groupIt == groupIt4)
	{
		std::cout << "After incrementing, groupIt is now equal to groupIt4 (pointing to the same position)."
				  << std::endl;
	}
	else
	{
		std::cout << "After incrementing, groupIt is NOT equal to groupIt4 (they should be equal now)." << std::endl;
	}

	// Test the relational operators
	std::cout << "\nTest relational operators" << std::endl;

	// Reinitialize groupIt before testing the relational operators
	groupIt = GroupIterator<std::vector<int>::iterator>(vec.begin(), 3);
	GroupIterator<std::vector<int>::iterator> groupIt5(vec.begin() + 6, 3); // Points to the third group (7, 8, 9)

	// Test the less-than operator
	if (groupIt < groupIt5)
	{
		std::cout << "groupIt is less than groupIt5 (pointing to an earlier position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt is NOT less than groupIt5 (they should be in different positions)." << std::endl;
	}

	// Test the less-than-or-equal operator
	if (groupIt <= groupIt3)
	{
		std::cout << "groupIt is less than or equal to groupIt3 (pointing to the same or an earlier position)."
				  << std::endl;
	}
	else
	{
		std::cout << "groupIt is NOT less than or equal to groupIt3 (they should be in the same position)."
				  << std::endl;
	}

	// Test the greater-than operator
	if (groupIt5 > groupIt)
	{
		std::cout << "groupIt5 is greater than groupIt (pointing to a later position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt5 is NOT greater than groupIt (they should be in different positions)." << std::endl;
	}

	// Test the greater-than-or-equal operator
	if (groupIt >= groupIt3)
	{
		std::cout << "groupIt is greater than or equal to groupIt3 (pointing to the same or a later position)."
				  << std::endl;
	}
	else
	{
		std::cout << "groupIt is NOT greater than or equal to groupIt3 (they should be in the same position)."
				  << std::endl;
	}

	testDereferenceOperator();

	// Call the test function to test arithmetic operators
	testArithmeticOperators();

	return 0;
}
