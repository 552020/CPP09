#include "PmergeMe.hpp"

template <typename C>
void PmergeMe::printContainer(C const &c)
{
	for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

template <typename RandomAccessIterator, typename Compare>
// clang-format off
void PmergeMe::binaryInsertionIntoMainChain(const std::vector<unsigned long long> &slicedJacobsthalDiff, std::list<PendChainNode<RandomAccessIterator> > &pendChain, std::list<RandomAccessIterator> &mainChain, Compare compare)
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
void PmergeMe::finalizeSorting(std::list<RandomAccessIterator> &mainChain, RandomAccessIterator first, std::size_t size)
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

template <typename RandomAccessIterator, typename Compare>
void PmergeMe::mergeInsertionSortImpl(RandomAccessIterator first,
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
void PmergeMe::mergeInsertionSort(RandomAccessIterator first,
								  RandomAccessIterator last,
								  Compare compare,
								  const std::vector<unsigned long long> &slicedJacobsthalDiff)
{
	// std::cout << "mergeInsertionSort" << std::endl;
	mergeInsertionSortImpl(makeGroupIterator(first, 1), makeGroupIterator(last, 1), compare, slicedJacobsthalDiff);
}

template <typename Container>
bool PmergeMe::isSorted(const Container &container)
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

template <typename T>
void PmergeMe::testDequeSort(std::deque<T> &deq,
							 std::vector<unsigned long long> &slicedJacobsthalDifferences,
							 bool print)
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
