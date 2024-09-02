#include "group_iterator_tests.hpp"
#include "PmergeMe.hpp"

std::vector<unsigned long long> generateJacobsthalNumbers(size_t n);

std::vector<unsigned long long> generateJacobsthalDifferences(size_t n);

template <typename RandomAccessIterator, typename Compare>
void mergeInsertionSort(RandomAccessIterator first,
						RandomAccessIterator last,
						Compare compare,
						const std::vector<unsigned long long> &slicedJacobsthalDiff);

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

void testIteratorCompatibility()
{
	PmergeMe pmergeMe;
	// Generate the full Jacobsthal differences vector
	std::vector<unsigned long long> fullJacobsthalDifferences = generateJacobsthalDifferences(65);

	// Create a slice of the full vector starting from index 2, following Marowenn's example
	std::vector<unsigned long long> slicedJacobsthalDifferences(fullJacobsthalDifferences.begin() + 2,
																fullJacobsthalDifferences.end());

	// Test with std::vector (Random Access Iterator)
	int vecArr[] = {5, 3, 2, 8, 7, 6, 1, 4};
	// int vecArr[] = {5, 3, 2};
	std::vector<int> vec(vecArr, vecArr + sizeof(vecArr) / sizeof(vecArr[0]));

	// Check the size of the vector before processing
	std::cout << "Vector size: " << vec.size() << std::endl;

	try
	{

		std::cout << "Testing with std::vector (Random Access Iterator):" << std::endl;
		// mergeInsertionSort(vec.begin(), vec.end(), std::less<int>());
		GroupIterator<std::vector<int>::iterator> it(vec.begin(), 3);
		std::cout << "GroupIterator initialized. Base: " << *it << std::endl;
		pmergeMe.mergeInsertionSort(vec.begin(), vec.end(), std::less<int>(), slicedJacobsthalDifferences);
		std::cout << "After mergeInsertionSort:" << std::endl;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	// Test with std::deque (Random Access Iterator)
	int dequeArr[] = {5, 3, 2, 8, 7, 6, 1, 4};
	std::deque<int> deq(dequeArr, dequeArr + sizeof(dequeArr) / sizeof(dequeArr[0]));

	std::cout << "Testing with std::deque (Random Access Iterator):" << std::endl;
	// mergeInsertionSort(deq.begin(), deq.end(), std::less<int>());
	pmergeMe.mergeInsertionSort(deq.begin(), deq.end(), std::less<int>(), slicedJacobsthalDifferences);
	for (std::deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	// Test with std::list (Bidirectional Iterator)
	int listArr[] = {5, 3, 8, 1, 2, 7, 4, 6};
	std::list<int> lst(listArr, listArr + sizeof(listArr) / sizeof(listArr[0]));

	std::cout << "Testing with std::list (Bidirectional Iterator):" << std::endl;
	std::cout << "Comment out the following lines, but the code will not compile!" << std::endl;
	// This should fail to compile because std::list's iterators are not Random Access Iterators. Uncommenting the
	// next 	line should lead to a compilation error. pmergeMe.mergeInsertionSort(lst.begin(), lst.end(),
	// std::less<int>());
	// TODO: Add a test so that we catch the compilation error
}

void testGroupIterator()
{
	testDefaultConstructor();
	testParameterizedConstructor();
	testIterSwap();
	testComparisonOperators();
	testRelationalOperators();
	testDereferenceOperator();
	testArithmeticOperators();
	testMakeGroupIterator();
	testIteratorCompatibility();
}