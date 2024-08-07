#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <iostream> // Required for standard input and output operations like std::cout.
#include <sstream>
#include <vector>
#include <algorithm> // Required for using std::iter_swap and other utility functions like sort, swap, etc.
#include <list>
#include <ctime>   // std::clock_t
#include <utility> // std::swap

class PmergeMe
{
  public:
	// PmergeMe(const std::vector<int> &numbers) : _vecNumbers(numbers);
	PmergeMe(const std::vector<int> &numbers);
	~PmergeMe();

	void sortVec();
	void sortList();

	template <typename C>
	void printContainer(C const &container);

  private:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);

	std::vector<int> _vecNumbers;
	std::list<int> _listNumbers;

	void sortVecInitialPairSortAndPrep();
};

// Template function defined outside the class, but within the same header file
template <typename C>
void PmergeMe::printContainer(C const &c)
{
	for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

#endif