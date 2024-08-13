#include "PmergeMe.hpp"
#include "utils.hpp"

PmergeMe::PmergeMe(const std::vector<int> &numbers)
	: _vecNumbers(numbers), _deqNumbers(), _durationVec(0), _durationDeq(0), _defaultPrecision(DEFAULT_PRECISION)
{
	_deqNumbers.assign(_vecNumbers.begin(), _vecNumbers.end());
	_jacDiffs = generateJacobsthalDifferences(65);
	_slicedJacDiffs.assign(_jacDiffs.begin() + 2, _jacDiffs.end());
}

PmergeMe::PmergeMe()
	: _vecNumbers(), _deqNumbers(), _durationVec(0), _durationDeq(0), _defaultPrecision(DEFAULT_PRECISION)
{
	// Containers are already initialized as empty by default
	_jacDiffs = generateJacobsthalDifferences(65);
	_slicedJacDiffs.assign(_jacDiffs.begin() + 2, _jacDiffs.end());
}

PmergeMe::~PmergeMe()
{
}

void PmergeMe::sortVec(bool print)
{
	if (print)
		printContainer(_vecNumbers, "Before");
	std::clock_t start = std::clock();
	mergeInsertionSort(_vecNumbers.begin(), _vecNumbers.end(), std::less<int>(), _slicedJacDiffs);
	std::clock_t end = std::clock();
	_durationVec = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	if (print)
	{
		printContainer(_vecNumbers, "After");
		printTiming(_vecNumbers, start, end, 6, "std::vector");
	}
}

void PmergeMe::sortDeque(bool print)
{
	if (print)
		printContainer(_deqNumbers, "Before");
	std::clock_t start = std::clock();
	mergeInsertionSort(_deqNumbers.begin(), _deqNumbers.end(), std::less<int>(), _slicedJacDiffs);
	std::clock_t end = std::clock();
	_durationDeq = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	if (print)
	{
		printContainer(_deqNumbers, "After");
		printTiming(_deqNumbers, start, end, 6, "std::deque");
	}
}

void PmergeMe::sortVecAndDeque()
{
	printContainer(_vecNumbers, "Before");
	sortVec(false);
	printContainer(_vecNumbers, "After");
	sortDeque(false);
	if (!isSorted(_vecNumbers) || !isSorted(_deqNumbers))
		std::cout << "Error: The vector or deque is not sorted" << std::endl;
	printTimingDuration(_vecNumbers, _durationVec, _defaultPrecision, "std::vector");
	printTimingDuration(_deqNumbers, _durationDeq, _defaultPrecision, "std::deque");
}

void PmergeMe::testVectorSort(std::vector<int> &vec, bool print)
{
	if (print)
		printContainer(vec, "Before");
	clock_t start = clock();
	mergeInsertionSort(vec.begin(), vec.end(), std::less<int>(), _slicedJacDiffs);
	clock_t end = clock();
	if (print)
		printContainer(vec, "After");
	else
	{
		bool sorted = isSorted(vec);
		std::cout << "Is it sorted? " << (sorted ? "Yes" : "No") << std::endl;
	}
	printTiming(vec, start, end, 6, "std::vector");
}

void PmergeMe::multipleTestVectorSort(int numTests, int minElements, int maxElements, bool print)
{
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
	std::vector<int> vec;
	// Generate random positive integers
	for (int i = 0; i < numTests; ++i)
	{
		int numElements = minElements + rand() % (maxElements - minElements + 1);
		std::vector<int> vec;
		for (int j = 0; j < numElements; ++j)
			vec.push_back(rand() % 10000 + 1); // Random number between 1 and 10000

		std::cout << "Test #" << i + 1 << ": Vector with " << numElements << " elements" << std::endl;
		testVectorSort(vec, print);
		std::cout << std::endl;
	}
};

void PmergeMe::testDequeSort(std::deque<int> &deq, bool print)
{
	if (print)
		printContainer(deq, "Before");
	clock_t start = clock();
	mergeInsertionSort(deq.begin(), deq.end(), std::less<int>(), _slicedJacDiffs);
	clock_t end = clock();
	if (print)
		printContainer(deq, "After");
	else
	{
		bool sorted = isSorted(deq);
		std::cout << "Is it sorted? " << (sorted ? "Yes" : "No") << std::endl;
	}
	printTiming(deq, start, end, 6, "std::deque");
}

void PmergeMe::multipleTestDequeSort(int numTests, int minElements, int maxElements, bool print)
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
		testDequeSort(deq, print);
		std::cout << std::endl;
	}
}

template <typename T>
void PmergeMe::compareVecAndDequeSort(std::vector<T> &vec,
									  std::vector<unsigned long long> &slicedJacobsthaDifference,
									  bool print)
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

void PmergeMe::multipleCompareVecAndDequeSort(int numTests,
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
