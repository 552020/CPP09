// GroupIterator.tpp
#include "GroupIterator.hpp"

template <typename Iterator>
GroupIterator<Iterator>::GroupIterator() : _it(), _size(0)
{
}

template <typename Iterator>
GroupIterator<Iterator>::GroupIterator(Iterator it, std::size_t size) : _it(it), _size(size)
{
}

// Copy constructor
template <typename Iterator>
GroupIterator<Iterator>::GroupIterator(const GroupIterator &other) : _it(other._it), _size(other._size)
{
}

// Copy assignment operator
template <typename Iterator>
GroupIterator<Iterator> &GroupIterator<Iterator>::operator=(const GroupIterator &other)
{
	if (this != &other)
	{
		_it = other._it;
		_size = other._size;
	}
	return *this;
}

// Destructor
template <typename Iterator>
GroupIterator<Iterator>::~GroupIterator()
{
}

// Member access
template <typename Iterator>
typename GroupIterator<Iterator>::iterator_type GroupIterator<Iterator>::base() const
{
	return _it;
}

template <typename Iterator>
std::size_t GroupIterator<Iterator>::size() const
{
	return _size;
}

// Element access
template <typename Iterator>
typename GroupIterator<Iterator>::reference GroupIterator<Iterator>::operator*() const
{
	return _it[_size - 1];
}

template <typename Iterator>
typename GroupIterator<Iterator>::pointer GroupIterator<Iterator>::operator->() const
{
	return &operator*();
}

// Increment/decrement operators
template <typename Iterator>
GroupIterator<Iterator> &GroupIterator<Iterator>::operator++()
{
	_it += _size;
	return *this;
}

template <typename Iterator>
GroupIterator<Iterator> GroupIterator<Iterator>::operator++(int)
{
	GroupIterator tmp = *this;
	++(*this);
	return tmp;
}

template <typename Iterator>
GroupIterator<Iterator> &GroupIterator<Iterator>::operator--()
{
	_it -= _size;
	return *this;
}

template <typename Iterator>
GroupIterator<Iterator> GroupIterator<Iterator>::operator--(int)
{
	GroupIterator tmp = *this;
	--(*this);
	return tmp;
}

// Addition/subtraction assignment operators
template <typename Iterator>
GroupIterator<Iterator> &GroupIterator<Iterator>::operator+=(std::size_t increment)
{
	_it += _size * increment;
	return *this;
}

template <typename Iterator>
GroupIterator<Iterator> &GroupIterator<Iterator>::operator-=(std::size_t increment)
{
	_it -= _size * increment;
	return *this;
}

// iter_swap function template. It's just a wrapper around std::swap_ranges. And it's used only one time in the code

template <typename Iterator1, typename Iterator2>
void iter_swap(GroupIterator<Iterator1> lhs, GroupIterator<Iterator2> rhs)
{
	std::swap_ranges(lhs.base(), lhs.base() + lhs.size(), rhs.base());
};

// Comparison operators

template <typename Iterator1, typename Iterator2>
bool operator==(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() == rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator!=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() != rhs.base();
}

// Relational operators

template <typename Iterator1, typename Iterator2>
bool operator<(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() < rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator<=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() <= rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator>(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() > rhs.base();
}

template <typename Iterator1, typename Iterator2>
bool operator>=(const GroupIterator<Iterator1> &lhs, const GroupIterator<Iterator2> &rhs)
{
	return lhs.base() >= rhs.base();
}

// Arithmetic operators

// Addition operator (iterator + numGroups)
// Moves the iterator forward by a specified number of groups.
template <typename Iterator>
GroupIterator<Iterator> operator+(GroupIterator<Iterator> it, std::size_t numGroups)
{
	return it += numGroups; // Advance the iterator by numGroups and return the result.
}

// template <typename Iterator>
// GroupIterator<Iterator> operator+(GroupIterator<Iterator> it, std::size_t numGroups)
// {
// 	std::cout << "Before operator+: _it = " << &(*it) << ", numGroups = " << numGroups << std::endl;
// 	it += numGroups;
// 	std::cout << "After operator+: _it = " << &(*it) << std::endl;
// 	return it;
// }

// Addition operator (numGroups + iterator)
// This is a convenience overload that allows the numGroups to come first.
template <typename Iterator>
GroupIterator<Iterator> operator+(std::size_t numGroups, GroupIterator<Iterator> it)
{
	return it += numGroups; // Advance the iterator by numGroups and return the result.
}

// template <typename Iterator>
// GroupIterator<Iterator> operator+(std::size_t numGroups, GroupIterator<Iterator> it)
// {
// 	std::cout << "Before operator+(numGroups, iterator): _it = " << &(*it) << ", numGroups = " << numGroups
// 			  << std::endl;
// 	it += numGroups;
// 	std::cout << "After operator+(numGroups, iterator): _it = " << &(*it) << std::endl;
// 	return it;
// }

// Subtraction operator (iterator - numGroups)
// Moves the iterator backward by a specified number of groups.
template <typename Iterator>
GroupIterator<Iterator> operator-(GroupIterator<Iterator> it, std::size_t numGroups)
{
	return it -= numGroups; // Move the iterator back by numGroups and return the result.
}

// template <typename Iterator>
// GroupIterator<Iterator> operator-(GroupIterator<Iterator> it, std::size_t numGroups)
// {
// 	std::cout << "Before operator-: _it = " << &(*it) << ", numGroups = " << numGroups << std::endl;
// 	it -= numGroups;
// 	std::cout << "After operator-: _it = " << &(*it) << std::endl;
// 	return it;
// }

// Subtraction operator (iterator1 - iterator2)
// Calculates the difference in the number of groups between two iterators.
template <typename Iterator>
typename GroupIterator<Iterator>::difference_type operator-(const GroupIterator<Iterator> &lhs,
															const GroupIterator<Iterator> &rhs)
{
	return (lhs.base() - rhs.base()) / lhs.size(); // Calculate how many groups separate lhs and rhs.
}

// template <typename Iterator>
// typename GroupIterator<Iterator>::difference_type operator-(const GroupIterator<Iterator> &lhs,
// 															const GroupIterator<Iterator> &rhs)
// {
// 	std::ptrdiff_t diff = (lhs.base() - rhs.base()) / lhs.size();
// 	std::cout << "Difference operator-: lhs.base() = " << &(*lhs) << ", rhs.base() = " << &(*rhs) << ", diff = " << diff
// 			  << " groups" << std::endl;
// 	return diff;
// }

// Extra constructors
// Construction function for base iterator
template <typename Iterator>
GroupIterator<Iterator> makeGroupIterator(Iterator it, std::size_t size)
{
	return GroupIterator<Iterator>(it, size);
}

// Construction function for existing GroupIterator
template <typename Iterator>
GroupIterator<Iterator> makeGroupIterator(GroupIterator<Iterator> it, std::size_t size)
{
	// Compute the new size
	std::size_t new_size = size * it.size();

	// Create and return a new GroupIterator
	return GroupIterator<Iterator>(it.base(), new_size);
}
