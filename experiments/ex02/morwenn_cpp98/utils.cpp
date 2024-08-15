#include "utils.hpp"

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

void testJacobsthalDifferencesVector(size_t skipCount)
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
