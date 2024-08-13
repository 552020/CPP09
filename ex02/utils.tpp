#include "utils.hpp"

template <typename C>
void printContainer(C const &c, std::string str)
{
	std::cout << str << ": ";
	for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

template <typename Container>
void printTiming(
	const Container &container, clock_t start, clock_t end, int precision, const std::string &containerType)
{
	std::cout << std::fixed << std::setprecision(precision);
	double timeTaken = double(end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to process a range of " << container.size() << " elements with " << containerType << ": "
			  << timeTaken << " seconds" << std::endl;
}

template <typename Container>
void printTimingDuration(const Container &container, double duration, int precision, const std::string &containerType)
{
	std::cout << std::fixed << std::setprecision(precision);
	std::cout << "Time to process a range of " << container.size() << " elements with " << containerType << ": "
			  << duration << " seconds" << std::endl;
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
