#include "PmergeMe.hpp"

PmergeMe::PmergeMe(const std::vector<int> &numbers) : _vecNumbers(numbers)
{
	_listNumbers.assign(_vecNumbers.begin(), _vecNumbers.end());
}
PmergeMe::~PmergeMe()
{
}

void PmergeMe::sortVec()
{
	// std::cout << "Before: ";
	// printContainer(_vecNumbers);

	std::clock_t start = std::clock();
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "After:\n";
	printContainer(_vecNumbers);
	std::cout << "Time to process a range of " << _vecNumbers.size() << " elements: " << duration << " seconds"
			  << std::endl;
}

void PmergeMe::sortList()
{
	// std::cout << "Before: ";
	// printContainer(_listNumbers);

	std::clock_t start = std::clock();
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "After:\n";
	printContainer(_listNumbers);
	std::cout << "Time to process a range of " << _listNumbers.size() << " elements: " << duration << " seconds"
			  << std::endl;
}

void PmergeMe::sortVecInitialPairSortAndPrep()
{
	std::vector<int>::iterator first = _vecNumbers.begin();
	std::vector<int>::iterator last = _vecNumbers.end();
	bool has_stray = _vecNumbers.size() % 2 != 0;
	if (has_stray)
		--last;
	for (std::vector<int>::iterator it = first; it != last; it += 2)
	{
		if (*(it + 1) > *it) // Now it ensures descending order
			// std::swap(*it, *(it + 1));
			std::iter_swap(it, it + 1);
	}
}

// void initialPairSortingAndPreparation(std::vector<int> &vec)
// {
// 	// Array to store the larger elements from each pair for median finding
// 	std::vector<int> largerElements;

// 	for (size_t i = 0; i < vec.size(); i += 2)
// 	{
// 		// Ensure we have a pair to consider
// 		if (i + 1 < vec.size())
// 		{
// 			if (vec[i] > vec[i + 1])
// 			{
// 				std::swap(vec[i], vec[i + 1]);
// 			}
// 			// Assuming we are interested in the larger element for median finding
// 			largerElements.push_back(vec[i + 1]);
// 		}
// 		else
// 		{
// 			// For an odd number of elements, consider the last one directly
// 			largerElements.push_back(vec[i]);
// 		}
// 	}

// 	// At this point, `largerElements` contains elements for median finding.
// 	// For the sake of demonstration, let's pretend we sort and pick the median.
// 	// Note: In a true implementation, you might use a more efficient selection algorithm.
// 	std::sort(largerElements.begin(), largerElements.end());
// 	int median = largerElements[largerElements.size() / 2]; // Simplistic median finding

// 	std::cout << "Preliminary Median: " << median << std::endl;

// 	// Next steps would involve using this median for further partitioning or sorting.
// }
