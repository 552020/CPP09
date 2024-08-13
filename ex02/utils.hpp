#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <sstream>

template <typename RandomAccessIterator>
struct PendChainNode;

bool isNumber(const std::string &input);

bool checkInputAndFillNumersVec(int argc, char **argv, std::vector<int> &numbers);

std::vector<unsigned long long> generateJacobsthalNumbers(size_t n);
std::vector<unsigned long long> generateJacobsthalDifferences(size_t n);
void testJacobsthalDifferencesVector(size_t skipCount);

template <typename C>
void printContainer(C const &container, std::string str);

template <typename Container>
void printTiming(
	const Container &container, clock_t start, clock_t end, int precision, const std::string &containerType);

template <typename Container>
void printTimingDuration(const Container &container, double duration, int precision, const std::string &containerType);
template <typename RandomAccessIterator>
void printMainChain(const std::list<RandomAccessIterator> &mainChain);

template <typename RandomAccessIterator>
// clang-format off
void printPendChain(const std::list<PendChainNode<RandomAccessIterator> > &pendChain);

template <typename RandomAccessIterator>
void printPendChainWithNext(const std::list<PendChainNode<RandomAccessIterator> > &pendChain, const std::list<RandomAccessIterator> &mainChain);
// clang-format on

#include "utils.tpp"

#endif