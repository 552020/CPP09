#include <iostream>
#include <vector>
#include <algorithm> // For std::sort

void insertElement(std::vector<int> &arr, int *elements, size_t n)
{
	for (size_t i = 0; i < n; ++i)
	{
		arr.push_back(elements[i]);
	}
}

void printArray(std::vector<int> &arr)
{
	for (size_t i = 0; i < arr.size(); ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

// Helper function to sort elements pairwise
void sortPairs(std::vector<int> &arr)
{
	for (size_t i = 0; i + 1 < arr.size(); i += 2)
	{
		if (arr[i] > arr[i + 1])
		{
			std::swap(arr[i], arr[i + 1]);
		}
	}
}

void splitSortedPairs(std::vector<int> &arr, std::vector<int> &mainChain, std::vector<int> &pend)
{
	for (size_t i = 0; i < arr.size(); i += 2)
	{
		// Assuming arr is already sorted in pairs, take the second (higher) element for the main chain
		if (i + 1 < arr.size())
		{ // Check if there's a pair
			mainChain.push_back(std::max(arr[i], arr[i + 1]));
			pend.push_back(std::min(arr[i], arr[i + 1]));
		}
		else
		{ // If odd number of elements, the last element goes to pend
			pend.push_back(arr[i]);
		}
	}
}

void generateJacobsthalNumbers(int n, std::vector<int> &numbers)
{
	numbers.push_back(0); // J(0)
	if (n > 0)
	{
		numbers.push_back(1); // J(1)
		for (int i = 2; i < n; ++i)
		{ // Generate up to the n-th number, excluding since we start from 0
			numbers.push_back(numbers[i - 1] + 2 * numbers[i - 2]);
		}
	}
}

// Select next pend element to insert into main chain
int selectPendElement(std::vector<int> &pend, std::vector<int> &jacobsthalNumbers)
{
	static int lastSelectedIndex = -1;
	int Index = ++lastSelectedIndex;
	if (lastSelectedIndex < jacobsthalNumbers.size())
	{
		return jacobsthalNumbers[Index];
	}
	else
	{
		lastSelectedIndex = -1;
		return -1;
	}
}

int determineInsertPosition(std::vector<int> &mainChain, int pendElement)
{
	int insertPosition = 0;
	while (insertPosition < mainChain.size() && mainChain[insertPosition] < pendElement)
	{
		++insertPosition;
	}
	return insertPosition;
}

void insertPendElement(std::vector<int> &mainChain, std::vector<int> &pend, std::vector<int> &jacobsthalNumbers)
{
	int pendElement = selectPendElement(pend, jacobsthalNumbers);
	if (pendElement == -1)
	{
		return;
	}
	int insertPosition = 0;
	insertPosition = determineInsertPosition(mainChain, pendElement);
	mainChain.insert(mainChain.begin() + insertPosition, pendElement);
}

// Main Ford-Johnson merge-insertion sort function
void mergeInsertionSort(std::vector<int> &arr)
{
	std::vector<int> mainChain;
	std::vector<int> pend;
	std::vector<int> jacobsthalNumbers;
	// Step 1: Sort elements pairwise
	sortPairs(arr);
	// Step 2: Split sorted pairs into main chain and pend
	splitSortedPairs(arr, mainChain, pend);
	std::cout << "Main chain: ";
	printArray(mainChain);
	std::cout << "Pend: ";
	printArray(pend);
	generateJacobsthalNumbers(pend.size(), jacobsthalNumbers);
	std::cout << "Jacobsthal numbers: ";
	printArray(jacobsthalNumbers);

	// Placeholder for further steps
}

int main()
{
	int elements[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
	std::vector<int> arr;

	insertElement(arr, elements, sizeof(elements) / sizeof(elements[0]));

	mergeInsertionSort(arr);
	printArray(arr);

	return 0;
}
