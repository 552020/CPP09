#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <ctime>   // std::clock_t
#include <utility> // std::swap

class PmergeMe
{
  public:
	PmergeMe();
	~PmergeMe();

	void sortVec(std::string &input);
	void sortList(std::string &input);

  private:
	std::vector<int> _vecNumbers;
	std::list<int> _listNumbers;
	PmergeMe(const PmergeMe &rhs);
	PmergeMe &operator=(const PmergeMe &rhs);

	template <typename C>
	void fillContainer(std::string const &input, C &container);
	template <typename C>
	void printContainer(C const &container);
	void initialPariSortingAndPreparation();
};

#include "PmergeMe.tpp"

#endif