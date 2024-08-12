#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <ctime> // For clock_t, clock, CLOCKS_PER_SEC
#include "GroupIterator.hpp"

template <typename RandomAccessIterator>
struct PendChainNode
{
	// The iterator pointing to the current element
	RandomAccessIterator it;
	// Iterator to the next element in the main chain
	// next is probably a deceptive name, cause next is not pointing to the next node in the chain
	typename std::list<RandomAccessIterator>::iterator next;
};

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

template <typename RandomAccessIterator, typename Compare>
// clang-format off
void binaryInsertionIntoMainChain(const std::vector<unsigned long long> &slicedJacobsthalDiff, std::list<PendChainNode<RandomAccessIterator> > &pendChain, std::list<RandomAccessIterator> &mainChain, Compare compare)
// clang-format on
{
	// clang-format off
	typedef typename std::list<PendChainNode<RandomAccessIterator> >::iterator PendIt;
	// clang-format on
	typedef typename std::list<RandomAccessIterator>::iterator MainIt;

	for (size_t k = 0; k < slicedJacobsthalDiff.size(); ++k)
	{
		unsigned long long dist = slicedJacobsthalDiff[k];
		if (dist >= pendChain.size())
			break;
		PendIt it = pendChain.begin();
		std::advance(it, dist);
		while (true)
		{

			// MainIt insertionPoint = std::upper_bound(mainChain.begin(), mainChain.end(), *it->it, compare);
			// MainIt insertionPoint = std::upper_bound(mainChain.begin(), it->next, *it->it, compare);
			// Manual loop to find insertion point instead of std::upper_bound
			MainIt insertionPoint = mainChain.begin();
			for (; insertionPoint != it->next; ++insertionPoint)
			{
				if (compare(*it->it, **insertionPoint))
				{
					break;
				}
			}

			mainChain.insert(insertionPoint, it->it);
			it = pendChain.erase(it);
			if (it == pendChain.begin())
				break;
			--it;
		}
	}
	while (!pendChain.empty())
	{
		// clang-format off
		typename std::list<PendChainNode<RandomAccessIterator> >::iterator it = --pendChain.end();
		// clang-format on
		typename std::list<RandomAccessIterator>::iterator insertionPoint = mainChain.begin();
		for (; insertionPoint != mainChain.end(); ++insertionPoint)
		{
			if (compare(*it->it, **insertionPoint))
				break;
		}
		mainChain.insert(insertionPoint, it->it);
		pendChain.pop_back();
	}
}

template <typename RandomAccessIterator>
void finalizeSorting(std::list<RandomAccessIterator> &mainChain, RandomAccessIterator first, std::size_t size)
{
	// std::cout << "Main Chain before caching: " << std::endl;
	// for (typename std::list<RandomAccessIterator>::iterator it = mainChain.begin(); it != mainChain.end(); ++it)
	// {
	// 	typename RandomAccessIterator::iterator_type base_it = (*it).base();
	// 	std::cout << "Value: " << *base_it << ", Address: " << static_cast<void *>(std::addressof(*base_it))
	// 	  << std::endl;
	// }
	std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> cache;
	cache.reserve(size);

	// std::cout << "Starting finalizeSorting. Cache initialized with size: " << size << std::endl;
	for (typename std::list<RandomAccessIterator>::iterator it = mainChain.begin(); it != mainChain.end(); ++it)
	{
		typename RandomAccessIterator::iterator_type begin = (*it).base();
		typename RandomAccessIterator::iterator_type end = begin + (*it).size();

		// std::cout << "Begin: " << *begin << ", End: " << *(end - 1) << ", Size: " << it->size() << std::endl;
		// std::cout << "Copying range from address " << static_cast<void *>(&(*begin)) << " to "
		//   << static_cast<void *>(&(*(end - 1))) << std::endl;

		for (; begin != end; ++begin)
		{
			// std::cout << "Pushing " << *begin << " from address " << static_cast<void *>(&(*begin)) << " to cache."
			// 		  << std::endl;
			cache.push_back(*begin);
		}
	}
	// std::cout << "Finished copying to cache. Cache size: " << cache.size() << std::endl;

	if (cache.size() != size)
	{
		// std::cerr << "Warning: Cache size (" << cache.size() << ") does not match expected size (" << size << ")."
		// 		  << std::endl;
	}

	typename std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>::iterator cacheIt =
		cache.begin();
	// std::cout << "Cache: ";
	// for (; cacheIt != cache.end(); ++cacheIt)
	// {
	// 	std::cout << *cacheIt << " ";
	// }
	// std::cout << std::endl;

	// std::cout << "Original container before restoring:" << std::endl;
	for (typename RandomAccessIterator::iterator_type it = first.base(); it != (first + size).base(); ++it)
	{
		// std::cout << "Value: " << *it << ", Address: " << static_cast<void *>(std::addressof(*it)) << std::endl;
	}
	// std::cout << std::endl;

	// Copy values from the cache back to the original container
	typename RandomAccessIterator::iterator_type originalIt = first.base();
	cacheIt = cache.begin();

	while (cacheIt != cache.end())
	{
		// std::cout << "Copying " << *cacheIt << " from cache to address "
		// 		  << static_cast<void *>(std::addressof(*originalIt)) << std::endl;
		*originalIt = *cacheIt;
		++originalIt;
		++cacheIt;
	}

	// std::cout << "Original container after restoring:" << std::endl;
	// for (typename RandomAccessIterator::iterator_type it = first.base(); it != (first + size).base(); ++it)
	// {
	// 	std::cout << "Value: " << *it << ", Address: " << static_cast<void *>(std::addressof(*it)) << std::endl;
	// }
	// std::cout << "Finished finalizeSorting." << std::endl;
}

//**  TEST FUNCTIONS **//

void testJacobsthalDiffemrencesVector(size_t skipCount)
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

template <typename RandomAccessIterator>
void printMainChain(const std::list<RandomAccessIterator> &mainChain)
{
	std::cout << "Main Chain: ";
	for (typename std::list<RandomAccessIterator>::const_iterator it = mainChain.begin(); it != mainChain.end(); ++it)
	{
		std::cout << *(*it) << " ";
	}
	std::cout << std::endl;
}

template <typename RandomAccessIterator>
// clang-format off
void printPendChain(const std::list<PendChainNode<RandomAccessIterator> > &pendChain)
// clang-format on
{
	std::cout << "Pend Chain: ";
	// clang-format off
	for (typename std::list<PendChainNode<RandomAccessIterator> >::const_iterator it = pendChain.begin();
		// clang-format on
		it != pendChain.end();
		++it)
	{
		// std::cout << *(*(it->it)) << " ";
		std::cout << *(it->it) << " "; // Single dereference
	}
	std::cout << std::endl;
}

template <typename RandomAccessIterator>
// clang-format off
void printPendChainWithNext(const std::list<PendChainNode<RandomAccessIterator> > &pendChain, const std::list<RandomAccessIterator> &mainChain)
// clang-format on
{
	std::cout << "Pend Chain with Next: ";
	// clang-format off
	for (typename std::list<PendChainNode<RandomAccessIterator> >::const_iterator it = pendChain.begin();
		 it != pendChain.end();
		 ++it)
	// clang-format on
	{
		// std::cout << "[Value: " << *(*(it->it)) << ", Next: ";
		std::cout << "[Value: " << *(it->it) << ", Next: ";

		// if (it->next != typename std::list<RandomAccessIterator>::iterator())
		if (it->next != mainChain.end())

		{
			std::cout << **(it->next);
		}
		else
		{
			std::cout << "End";
		}
		std::cout << "] ";
	}
	std::cout << std::endl;
}

template <typename RandomAccessIterator, typename Compare>
void mergeInsertionSortImpl(RandomAccessIterator first,
							RandomAccessIterator last,
							Compare compare,
							const std::vector<unsigned long long> &slicedJacobsthalDiff)
{
	// std::cout << "mergeInsertionSortImpl: first = " << *first << ", last = " << *(last - 1) << std::endl;

	using std::iter_swap;

	// Calculate the distance (size) between first and last iterators
	typename std::iterator_traits<RandomAccessIterator>::difference_type size = std::distance(first, last);
	// Debugging prints
	// std::cout << "Size: " << size << std::endl;
	// std::cout << "\033[31mFirst iterator points to value: " << *first
	// 		  << " at address: " << static_cast<void *>(&(*first)) << "\033[0m" << std::endl;

	// std::cout << "\033[31mLast iterator points to value: " << *(last - 1)
	// 		  << " at address: " << static_cast<void *>(&(*(last - 1))) << "\033[0m" << std::endl;

	// std::cout << "\033[31mDistance (size) between first and last: " << size << "\033[0m" << std::endl;
	if (size < 2)
	{
		// std::cout << "Base case reached with size < 2, returning..." << std::endl;
		return;
	}

	bool hasStray = (size % 2 != 0);

	RandomAccessIterator end = last;
	if (hasStray)
		--end; // Equivalent to std::prev(last) in C++98

	// Iterate over pairs of elements
	// std::cout << "Iterating over pairs of elements ..." << std::endl;
	for (RandomAccessIterator it = first; it != end; it += 2)
	{
		// std::cout << "Current pair: " << *it << " and " << *(it + 1) << std::endl;
		// Compare the current pair and swap if out of order
		if (compare(*(it + 1), *it))
		{
			// std::cout << "Swapping " << *it << " and " << *(it + 1) << std::endl;
			iter_swap(it, it + 1);
		}
		else
		{
			// std::cout << "No need to swap " << *it << " and " << *(it + 1) << std::endl;
		}
	}
	// Debugging the recursive call
	// std::cout << "Calling mergeInsertionSortImpl recursively with first to end" << std::endl;

	// Recursively sort pairs by their maximum value
	// std::cout << "first: " << *first << std::endl;
	// std::cout << "end: " << *(end - 1) << std::endl;
	mergeInsertionSort(makeGroupIterator(first, 2), makeGroupIterator(end, 2), compare, slicedJacobsthalDiff);

	// print size in yellow
	// std::cout << "\033[33mSize: " << size << "\033[0m" << std::endl;

	typedef PendChainNode<RandomAccessIterator> NodeType;
	std::list<RandomAccessIterator> mainChain;
	std::list<NodeType> pendChain;

	// Initialize the mainChain with the first two elements
	// This is done in morwenn's code with the following line:
	// 	std::list<RandomAccessIterator> chain = {first, std::next(first)};
	mainChain.push_back(first);
	// std::cout << "first: " << *first << std::endl;
	RandomAccessIterator second = first + 1;
	// std::cout << "second: " << *second << std::endl;
	mainChain.push_back(second);

	// Initialize the pendChain with the rest
	// first is the first element on the data structure containing the numbers
	// end is the last one, or the previous one if we have a stray
	for (RandomAccessIterator it = first + 2; it != end; it += 2)
	{
		RandomAccessIterator nextIt = it + 1;
		// .end() returns an iterator to the element following the last element of the list
		// .insert() insert an element into the list before the position pointed to by the interator provided
		// (mainChain.end(), the value inserted is nextIt)
		typename std::list<RandomAccessIterator>::iterator tmp = mainChain.insert(mainChain.end(), nextIt);
		NodeType node = {it, tmp};
		pendChain.push_back(node);
	}

	// If there is an odd element, handle it
	if (hasStray)
	{
		NodeType node = {end, mainChain.end()};
		pendChain.push_back(node);
		// std::cout << "pendChain, last element: " << *end << std::endl;
	}
	// Call the printing functions
	// printMainChain(mainChain);
	// printPendChain(pendChain);
	// printPendChainWithNext(pendChain, mainChain);

	binaryInsertionIntoMainChain(slicedJacobsthalDiff, pendChain, mainChain, compare);

	// std::cout << "After binaryInsertionIntoMainChain:" << std::endl;
	// printMainChain(mainChain);
	// printPendChain(pendChain);
	// printPendChainWithNext(pendChain, mainChain);

	finalizeSorting(mainChain, first, size);
}

template <typename RandomAccessIterator, typename Compare>
void mergeInsertionSort(RandomAccessIterator first,
						RandomAccessIterator last,
						Compare compare,
						const std::vector<unsigned long long> &slicedJacobsthalDiff)
{
	// std::cout << "mergeInsertionSort" << std::endl;
	mergeInsertionSortImpl(makeGroupIterator(first, 1), makeGroupIterator(last, 1), compare, slicedJacobsthalDiff);
}

void testIteratorCompatibility()
{
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
		mergeInsertionSort(vec.begin(), vec.end(), std::less<int>(), slicedJacobsthalDifferences);
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
	mergeInsertionSort(deq.begin(), deq.end(), std::less<int>(), slicedJacobsthalDifferences);
	for (std::deque<int>::iterator it = deq.begin(); it != deq.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	// Test with std::list (Bidirectional Iterator)
	int listArr[] = {5, 3, 8, 1, 2, 7, 4, 6};
	std::list<int> lst(listArr, listArr + sizeof(listArr) / sizeof(listArr[0]));

	std::cout << "Testing with std::list (Bidirectional Iterator):" << std::endl;
	std::cout << "Comment out the following lines, but the code will not compile!" << std::endl;
	// This should fail to compile because std::list's iterators are not Random Access Iterators. Uncommenting the
	// next 	line should lead to a compilation error. mergeInsertionSort(lst.begin(), lst.end(),
	// std::less<int>());
	// TODO: Add a test so that we catch the compilation error
}

template <typename Container>
bool isSorted(const Container &container)
{
	if (container.empty())
	{
		return true; // An empty container is considered sorted.
	}

	typename Container::const_iterator it = container.begin();
	typename Container::const_iterator next_it = it;
	++next_it;

	for (; next_it != container.end(); ++it, ++next_it)
	{
		if (*it > *next_it)
		{
			return false; // Found an element that is greater than the next one.
		}
	}
	return true; // No elements were out of order.
}

void testGroupIteartor()
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

template <typename T>
void testVectorSort(std::vector<T> &vec, std::vector<unsigned long long> &slicedJacobsthalDifferences, bool print)
{
	if (print)
	{
		std::cout << "Before: ";
		for (typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	clock_t start = clock();
	mergeInsertionSort(vec.begin(), vec.end(), std::less<T>(), slicedJacobsthalDifferences);
	clock_t end = clock();
	if (print)
	{
		std::cout << "After: ";
		for (typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	// Check if the vector is sorted
	bool sorted = isSorted(vec);
	// Print if the vector is sorted or not
	std::cout << "Is it sorted? " << (sorted ? "Yes" : "No") << std::endl;
	// Calculate and print the time taken
	double timeTaken = double(end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << vec.size() << " elements with std::vector: " << timeTaken
			  << " seconds" << std::endl;
}

void multipleTestVectorSort(int numTests,
							int minElements,
							int maxElements,
							std::vector<unsigned long long> &slicedJacobsthalDifferences,
							bool print)
{
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
	std::vector<int> vec;
	// Generate random positive integers
	for (int i = 0; i < numTests; ++i)
	{
		int numElements = minElements + rand() % (maxElements - minElements + 1);
		std::vector<int> vec;
		for (int j = 0; j < numElements; ++j)
		{
			vec.push_back(rand() % 1000 + 1); // Random number between 1 and 1000
		}

		std::cout << "Test #" << i + 1 << ": Vector with " << numElements << " elements" << std::endl;
		testVectorSort(vec, slicedJacobsthalDifferences, print);
		std::cout << std::endl;
	}
}

// Function to test sorting a deque and print results
template <typename T>
void testDequeSort(std::deque<T> &deq, std::vector<unsigned long long> &slicedJacobsthalDifferences, bool print)
{
	if (print)
	{
		std::cout << "Before: ";
		for (typename std::deque<T>::iterator it = deq.begin(); it != deq.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	clock_t start = clock();
	mergeInsertionSort(deq.begin(), deq.end(), std::less<T>(), slicedJacobsthalDifferences);
	clock_t end = clock();
	if (print)
	{
		std::cout << "After: ";
		for (typename std::deque<T>::iterator it = deq.begin(); it != deq.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	// Check if the deque is sorted
	bool sorted = isSorted(deq);
	// Print if the deque is sorted or not
	std::cout << "Is it sorted? " << (sorted ? "Yes" : "No") << std::endl;
	// Calculate and print the time taken
	double timeTaken = double(end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << deq.size() << " elements with std::deque: " << timeTaken << " seconds"
			  << std::endl;
}

// Function to run multiple sorting tests on deques
void multipleTestDequeSort(int numTests,
						   int minElements,
						   int maxElements,
						   std::vector<unsigned long long> &slicedJacobsthalDifferences,
						   bool print)
{
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

	for (int i = 0; i < numTests; ++i)
	{
		int numElements = minElements + rand() % (maxElements - minElements + 1);
		std::deque<int> deq;

		// Generate random positive integers
		for (int j = 0; j < numElements; ++j)
		{
			deq.push_back(rand() % 1000 + 1); // Random number between 1 and 1000
		}

		std::cout << "Test #" << i + 1 << ": Deque with " << numElements << " elements" << std::endl;
		testDequeSort(deq, slicedJacobsthalDifferences, print);
		std::cout << std::endl;
	}
}

template <typename T>
void compareVecAndDequeSort(std::vector<T> &vec, std::vector<unsigned long long> &slicedJacobsthaDifference, bool print)
{
	if (print)
	{
		std::cout << "Before: ";
		for (typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	clock_t startVec = clock();

	mergeInsertionSort(vec.begin(), vec.end(), std::less<T>(), slicedJacobsthaDifference);
	clock_t endVec = clock();
	if (print)
	{
		std::cout << "After: ";
		for (typename std::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
	else
	{
		bool vecSorted = isSorted(vec);
		std::cout << "Is the vector sorted? " << (vecSorted ? "Yes" : "No") << std::endl;
	}
	// Calculate and print the time taken for vector sort
	double timeVec = double(endVec - startVec) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << vec.size() << " elements with std::vector: " << timeVec << " seconds"
			  << std::endl;
	std::deque<T> deq(vec.begin(), vec.end());
	clock_t startDeq = clock();
	mergeInsertionSort(deq.begin(), deq.end(), std::less<T>(), slicedJacobsthaDifference);
	clock_t endDeq = clock();
	if (!print)
	{
		bool deqSorted = isSorted(deq);
		std::cout << "Is the deque sorted? " << (deqSorted ? "Yes" : "No") << std::endl;
	}
	double timeDeq = double(endDeq - startDeq) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << deq.size() << " elements with std::deque: " << timeDeq << " seconds"
			  << std::endl;
}

void multipleCompareVecAndDequeSort(int numTests,
									int minElements,
									int maxElements,
									std::vector<unsigned long long> &slicedJacobsthalDifferences,
									bool print)
{
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

	for (int i = 0; i < numTests; ++i)
	{
		int numElements = minElements + rand() % (maxElements - minElements + 1);
		std::vector<int> vec;
		for (int j = 0; j < numElements; ++j)
		{
			vec.push_back(rand() % 1000 + 1); // Random number between 1 and 1000
		}

		std::cout << "Test #" << i + 1 << ": Comparing Vector and Deque with " << numElements << " elements"
				  << std::endl;
		compareVecAndDequeSort(vec, slicedJacobsthalDifferences, print);
		std::cout << std::endl;
	}
}

int main()
{

	// testGroupIteartor();

	// testJacobsthalDifferencesVector(2);

	std::vector<unsigned long long> fullJacobsthalDifferences = generateJacobsthalDifferences(65);

	std::vector<unsigned long long> slicedJacobsthalDifferences(fullJacobsthalDifferences.begin() + 2,
																fullJacobsthalDifferences.end());

	int vecArr[] = {7, 3, 9, 1, 5, 4, 8, 6, 2};
	std::vector<int> vec(vecArr, vecArr + sizeof(vecArr) / sizeof(vecArr[0]));

	// Perform merge-insertion sort
	// mergeInsertionSort(vec.begin(), vec.end(), std::less<int>(), slicedJacobsthalDifferences);
	// std::cout << "After mergeInsertionSort:" << std::endl;
	// std::cout << "Sorted: " << (isSorted(vec) ? "Yes" : "No") << std::endl;
	// testVectorSort(vec, slicedJacobsthalDifferences);
	// multipleTestVectorSort(5, 100, 10000, slicedJacobsthalDifferences, false);
	// multipleTestDequeSort(5, 100, 10000, slicedJacobsthalDifferences, false);
	// compareVecAndDequeSort(vec, slicedJacobsthalDifferences, true);
	multipleCompareVecAndDequeSort(5, 100, 10000, slicedJacobsthalDifferences, false);

	return 0;
}
