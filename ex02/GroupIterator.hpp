#ifndef GROUP_ITERATOR_HPP
#define GROUP_ITERATOR_HPP

#include <iterator>
#include <cstddef>

template <typename Iterator>
class GroupIterator
{
  private:
	Iterator _it;	   // The underlying (base) iterator that this class wraps.
	std::size_t _size; // The size of the group of elements that this iterator manages.

  public:
	GroupIterator(const GroupIterator &);
	GroupIterator &operator=(const GroupIterator &);

	// Define standard iterator traits to make this iterator compatible with STL algorithms.
	typedef std::random_access_iterator_tag iterator_category;
	typedef Iterator iterator_type;
	typedef typename std::iterator_traits<Iterator>::value_type value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef typename std::iterator_traits<Iterator>::pointer pointer;
	typedef typename std::iterator_traits<Iterator>::reference reference;

	// Constructors
	GroupIterator();
	GroupIterator(Iterator it, std::size_t size);

	// Destructor
	~GroupIterator();

	// Member access
	iterator_type base() const;
	std::size_t size() const;

	// Element access
	reference operator*() const;
	pointer operator->() const;

	// Increment/decrement operators
	GroupIterator &operator++();
	GroupIterator operator++(int);
	GroupIterator &operator--();
	GroupIterator operator--(int);

	// Addition/subtraction assignment operators
	GroupIterator &operator+=(std::size_t increment);
	GroupIterator &operator-=(std::size_t increment);
};

// Declaration of the iter_swap function template
template <typename Iterator1, typename Iterator2>
void iter_swap(GroupIterator<Iterator1> lhs, GroupIterator<Iterator2> rhs);

// Comparison operators
template <typename Iterator1, typename Iterator2>
bool operator==(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

template <typename Iterator1, typename Iterator2>
bool operator!=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

// Relational operators
template <typename Iterator1, typename Iterator2>
bool operator<(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

template <typename Iterator1, typename Iterator2>
bool operator<=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

template <typename Iterator1, typename Iterator2>
bool operator>(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

template <typename Iterator1, typename Iterator2>
bool operator>=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs);

// Arithmetic operators - see the implementation in the .tpp file for a detailed explanation
template <typename Iterator>
GroupIterator<Iterator> operator+(GroupIterator<Iterator> it, std::size_t numGroups);

template <typename Iterator>
GroupIterator<Iterator> operator+(std::size_t numGroups, GroupIterator<Iterator> it);

template <typename Iterator>
GroupIterator<Iterator> operator-(GroupIterator<Iterator> it, std::size_t numGroups);

template <typename Iterator>
typename GroupIterator<Iterator>::difference_type operator-(const GroupIterator<Iterator> &lhs,
															const GroupIterator<Iterator> &rhs);

#include "GroupIterator.tpp"

#endif
