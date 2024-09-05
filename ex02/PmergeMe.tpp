#include "PmergeMe.hpp"

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

			MainIt insertionPoint = mainChain.begin();
			for (; insertionPoint != it->next; ++insertionPoint)
			{
				if (compare(*it->it, **insertionPoint))
					break;
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
	std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> cache;
	cache.reserve(size);

	for (typename std::list<RandomAccessIterator>::iterator it = mainChain.begin(); it != mainChain.end(); ++it)
	{
		typename RandomAccessIterator::iterator_type begin = (*it).base();
		typename RandomAccessIterator::iterator_type end = begin + (*it).size();
		for (; begin != end; ++begin)
			cache.push_back(*begin);
	}

	if (cache.size() != size)
	{
	}

	typename std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>::iterator cacheIt =
		cache.begin();
	for (typename RandomAccessIterator::iterator_type it = first.base(); it != (first + size).base(); ++it)
	{
		// std::cout << "Value: " << *it << ", Address: " << static_cast<void *>(std::addressof(*it)) << std::endl;
	}

	typename RandomAccessIterator::iterator_type originalIt = first.base();
	cacheIt = cache.begin();

	while (cacheIt != cache.end())
	{
		*originalIt = *cacheIt;
		++originalIt;
		++cacheIt;
	}
}

template <typename RandomAccessIterator, typename Compare>
void PmergeMe::mergeInsertionSortImpl(RandomAccessIterator first,
									  RandomAccessIterator last,
									  Compare compare,
									  const std::vector<unsigned long long> &slicedJacobsthalDiff)
{
	using std::iter_swap;

	// Calculate the distance (size) between first and last iterators
	typename std::iterator_traits<RandomAccessIterator>::difference_type size = std::distance(first, last);
	if (size < 2)
	{
		return;
	}

	bool hasStray = (size % 2 != 0);

	RandomAccessIterator end = last;
	if (hasStray)
		--end; // Equivalent to std::prev(last) in C++98

	// Iterate over pairs of elements
	for (RandomAccessIterator it = first; it != end; it += 2)
	{
		if (compare(*(it + 1), *it))
		{
			iter_swap(it, it + 1);
		}
		else
		{
			// std::cout << "No need to swap " << *it << " and " << *(it + 1) << std::endl;
		}
	}
	mergeInsertionSort(makeGroupIterator(first, 2), makeGroupIterator(end, 2), compare, slicedJacobsthalDiff);

	typedef PendChainNode<RandomAccessIterator> NodeType;
	std::list<RandomAccessIterator> mainChain;
	std::list<NodeType> pendChain;

	mainChain.push_back(first);
	RandomAccessIterator second = first + 1;
	mainChain.push_back(second);
	for (RandomAccessIterator it = first + 2; it != end; it += 2)
	{
		RandomAccessIterator nextIt = it + 1;
		typename std::list<RandomAccessIterator>::iterator tmp = mainChain.insert(mainChain.end(), nextIt);
		NodeType node = {it, tmp};
		pendChain.push_back(node);
	}
	if (hasStray)
	{
		NodeType node = {end, mainChain.end()};
		pendChain.push_back(node);
	}
	binaryInsertionIntoMainChain(slicedJacobsthalDiff, pendChain, mainChain, compare);
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
