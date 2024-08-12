#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <list>
#include <iostream>

template <typename RandomAccessIterator>
struct PendChainNode;

std::vector<unsigned long long> generateJacobsthalNumbers(size_t n);
std::vector<unsigned long long> generateJacobsthalDifferences(size_t n);
void testJacobsthalDifferencesVector(size_t skipCount);
template <typename RandomAccessIterator>
void printMainChain(const std::list<RandomAccessIterator> &mainChain);
template <typename RandomAccessIterator>
// clang-format off
void printPendChain(const std::list<PendChainNode<RandomAccessIterator> > &pendChain);

template <typename RandomAccessIterator>
void printPendChainWithNext(const std::list<PendChainNode<RandomAccessIterator> > &pendChain, const std::list<RandomAccessIterator> &mainChain);
// clang-format on

#endif