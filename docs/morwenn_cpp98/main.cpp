#include <iostream>
#include <vector>
#include "GroupIterator.hpp"

std::vector<unsigned long long> generateJacobsthalNumbers(size_t n)
{
	std::vector<unsigned long long> jacobsthal(n);
	if (n == 0)
		return jacobsthal;

	jacobsthal[0] = 0;
	if (n == 1)
		return jacobsthal;

	jacobsthal[1] = 1;
	for (size_t i = 2; i < n; ++i)
	{
		jacobsthal[i] = jacobsthal[i - 1] + 2 * jacobsthal[i - 2];
	}

	return jacobsthal;
}

std::vector<unsigned long long> generateJacobsthalDifferences(size_t n)
{
	std::vector<unsigned long long> jacobsthal = generateJacobsthalNumbers(n);
	std::vector<unsigned long long> jacobsthalDiff(n - 1);

	for (size_t i = 1; i < n; ++i)
	{
		jacobsthalDiff[i - 1] = jacobsthal[i] - jacobsthal[i - 1];
	}

	return jacobsthalDiff;
}

void testJacobsthalDifferencesVector(size_t skipCount)
{
	// Hardcoded Jacobsthal differences
	const uint_least64_t hardcodedDifferencesArray[] = {2u,
														2u,
														6u,
														10u,
														22u,
														42u,
														86u,
														170u,
														342u,
														682u,
														1366u,
														2730u,
														5462u,
														10922u,
														21846u,
														43690u,
														87382u,
														174762u,
														349526u,
														699050u,
														1398102u,
														2796202u,
														5592406u,
														11184810u,
														22369622u,
														44739242u,
														89478486u,
														178956970u,
														357913942u,
														715827882u,
														1431655766u,
														2863311530u,
														5726623062u,
														11453246122u,
														22906492246u,
														45812984490u,
														91625968982u,
														183251937962u,
														366503875926u,
														733007751850u,
														1466015503702u,
														2932031007402u,
														5864062014806u,
														11728124029610u,
														23456248059222u,
														46912496118442u,
														93824992236886u,
														187649984473770u,
														375299968947542u,
														750599937895082u,
														1501199875790165u,
														3002399751580331u,
														6004799503160661u,
														12009599006321322u,
														24019198012642644u,
														48038396025285288u,
														96076792050570576u,
														192153584101141152u,
														384307168202282304u,
														768614336404564608u,
														1537228672809129216u,
														3074457345618258432u,
														6148914691236516864u};

	std::vector<unsigned long long> hardcodedDifferences(
		hardcodedDifferencesArray,
		hardcodedDifferencesArray + sizeof(hardcodedDifferencesArray) / sizeof(hardcodedDifferencesArray[0]));

	// Generate the Jacobsthal differences using the function
	std::vector<unsigned long long> generatedDifferences =
		generateJacobsthalDifferences(hardcodedDifferences.size() + 1);

	// Compare the generated differences with the hardcoded values
	bool success = true;
	for (size_t i = skipCount; i < hardcodedDifferences.size(); ++i)
	// for (size_t i = 0; i < hardcodedDifferences.size(); ++i)
	{

		size_t j = i + skipCount;

		if (static_cast<uint_least64_t>(generatedDifferences[j]) != hardcodedDifferences[i])
		{
			std::cout << "Mismatch at index " << i << ": Expected " << hardcodedDifferences[i] << ", got "
					  << generatedDifferences[j] << std::endl;
			success = false;
		}
	}

	// Print the result
	if (success)
	{
		std::cout << "Test passed: All generated differences match the hardcoded values." << std::endl;
	}
	else
	{
		std::cout << "Test failed: Some generated differences do not match the hardcoded values." << std::endl;
	}
}

void testDefaultConstructor()
{
	// Test the default constructor
	GroupIterator<std::vector<int>::iterator> defaultGroupIt;
	std::cout << "Default GroupIterator created. Size: " << defaultGroupIt.size() << std::endl;
}

void testParameterizedConstructor()
{
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

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
}

void testIterSwap()
{
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
}

void testComparisonOperators()
{
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Create two GroupIterator instances for different parts of the vectors
	GroupIterator<std::vector<int>::iterator> groupIt1(vec.begin(), 3);
	GroupIterator<std::vector<int>::iterator> groupIt2(vec.begin() + 3, 3);
	GroupIterator<std::vector<int>::iterator> groupIt3(vec.begin(), 3);

	// Dubug Information
	std::cout << "groupIt1 base: " << (groupIt1.base() - vec.begin()) << std::endl;
	std::cout << "groupIt2 base: " << (groupIt2.base() - vec.begin()) << std::endl;
	std::cout << "groupIt3 base: " << (groupIt3.base() - vec.begin()) << std::endl;

	// Print the internal details of the iterators
	std::cout << "groupIt1: _it = " << &(*groupIt1.base()) << ", _size = " << groupIt1.size() << std::endl;
	std::cout << "groupIt2: _it = " << &(*groupIt2.base()) << ", _size = " << groupIt2.size() << std::endl;
	std::cout << "groupIt3: _it = " << &(*groupIt3.base()) << ", _size = " << groupIt3.size() << std::endl;

	// Test the equality operator
	if (groupIt1 == groupIt3)
	{
		std::cout << "groupIt1 is equal to groupIt3 (pointing to the same position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt1 is NOT equal to groupIt3 (they should be equal)." << std::endl;
	}

	// Test the inequality operator
	if (groupIt1 != groupIt2)
	{
		std::cout << "groupIt1 is NOT equal to groupIt2 (pointing to different positions)." << std::endl;
	}
	else
	{
		std::cout << "groupIt1 is equal to groupIt2 (they should be different)." << std::endl;
	}

	// Move groupIt1 to the next group and compare again
	++groupIt1;

	if (groupIt1 == groupIt2)
	{
		std::cout << "After incrementing, groupIt1 is now equal to groupIt2 (pointing to the same position)."
				  << std::endl;
	}
	else
	{
		std::cout << "After incrementing, groupIt1 is NOT equal to groupIt2 (they should be equal now)." << std::endl;
	}
}

void testRelationalOperators()
{
	// Initialize a vector with elements 1 through 10
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Create two GroupIterator instances for different parts of the vectors
	GroupIterator<std::vector<int>::iterator> groupIt1(vec.begin(), 3);
	GroupIterator<std::vector<int>::iterator> groupIt2(vec.begin() + 3, 3);
	GroupIterator<std::vector<int>::iterator> groupIt3(vec.begin(), 3);

	// Test the less-than operator
	if (groupIt1 < groupIt2)
	{
		std::cout << "groupIt1 is less than groupIt2 (pointing to an earlier position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt1 is NOT less than groupIt2 (they should be in different positions)." << std::endl;
	}

	// Test the less-than-or-equal operator
	if (groupIt1 <= groupIt3)
	{
		std::cout << "groupIt1 is less than or equal to groupIt3 (pointing to the same or an earlier position)."
				  << std::endl;
	}
	else
	{
		std::cout << "groupIt1 is NOT less than or equal to groupIt3 (they should be in the same position)."
				  << std::endl;
	}

	// Test the greater-than operator
	if (groupIt2 > groupIt1)
	{
		std::cout << "groupIt2 is greater than groupIt1 (pointing to a later position)." << std::endl;
	}
	else
	{
		std::cout << "groupIt2 is NOT greater than groupIt1 (they should be in different positions)." << std::endl;
	}

	// Test the greater-than-or-equal operator
	if (groupIt1 >= groupIt3)
	{
		std::cout << "groupIt1 is greater than or equal to groupIt3 (pointing to the same or a later position)."
				  << std::endl;
	}
	else
	{
		std::cout << "groupIt1 is NOT greater than or equal to groupIt3 (they should be in the same position)."
				  << std::endl;
	}
}
void testMakeGroupIterator()
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// Create a GroupIterator instance using the makeGroupIterator function
	GroupIterator<std::vector<int>::iterator> groupIt = makeGroupIterator(vec.begin() + 2, 3);	// Points to {3, 4, 5}
	GroupIterator<std::vector<int>::iterator> groupIt2 = makeGroupIterator(vec.begin() + 5, 3); // Points to {6, 7, 8}

	std::cout << "Test makeGroupIterator function\n";

	std::cout << "Group starting at index 2 (size 3): ";
	for (std::size_t i = 0; i < 3; ++i)
	{
		std::cout << *(groupIt.base() + i) << " ";
	}
	std::cout << "Expected: 3 4 5" << std::endl;
	std::cout << std::endl;
	std::cout << "Group starting at index 5 (size 3): ";
	for (std::size_t i = 0; i < 3; ++i)
	{
		std::cout << *(groupIt2.base() + i) << " ";
	}

	std::cout << "Expected: 6 7 8" << std::endl;
	std::cout << std::endl;
}

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

	// testDefaultConstructor();

	// testParameterizedConstructor();

	// testIterSwap();

	// testComparisonOperators();

	// testRelationalOperators();

	// testDereferenceOperator();

	// testArithmeticOperators();

	// testMakeGroupIterator();

	testJacobsthalDifferencesVector(2);

	return 0;
}
