#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <iterator>
#include <vector>

template <typename Iterator>
class GroupIterator
{
  private:
	Iterator it_;	   // The underlying (base) iterator that this class wraps.
	std::size_t size_; // The size of the group of elements that this iterator manages.

  public:
	// Define standard iterator traits to make this iterator compatible with STL algorithms.

	typedef std::random_access_iterator_tag
		iterator_category;			// Iterator category indicating this is a random access iterator.
	typedef Iterator iterator_type; // Alias for the base iterator type.
	typedef typename std::iterator_traits<Iterator>::value_type
		value_type; // Type of the elements pointed to by the iterator.
	typedef typename std::iterator_traits<Iterator>::difference_type
		difference_type; // Type for representing the difference between two iterators.
	typedef typename std::iterator_traits<Iterator>::pointer
		pointer; // Pointer type to the elements pointed to by the iterator.
	typedef typename std::iterator_traits<Iterator>::reference
		reference; // Reference type to the elements pointed to by the iterator.

	// Constructors

	GroupIterator() : it_(), size_(0)
	{
	}

	GroupIterator(Iterator it, std::size_t size) : it_(it), size_(size)
	{
	}

	// Members access - These are basically getters for the private members, we have base and size, cause this is a
	// convention in the STL Returns the base iterator that this group iterator wraps around.

	iterator_type base() const
	{
		return it_;
	}

	// Returns the size of the group of elements managed by this iterator.

	std::size_t size() const
	{
		return size_;
	}

	// Element access

	reference operator*() const
	{
		return _it[_size - 1];
		// Dereference operator returns the last element in the current group.
		// Instead of returning the element the base iterator points to, it returns the last element in the group.
	}

	pointer operator->() const
	{
		return &operator*();
		// Arrow operator returns a pointer to the last element in the current group.
		// It uses the dereference operator to get the element, and then returns its address.
	}

	////////////////////////////////////////////////////////////
	// Increment/decrement operators

	// Pre-increment: Moves the iterator forward by one group.
	// Advances the base iterator by the size of the group and returns a reference to the updated iterator.
	GroupIterator &operator++()
	{
		it_ += size_;
		return *this;
	}

	// Post-increment: Moves the iterator forward by one group, but returns the original iterator before the move.
	// Saves the current state, increments the iterator, and returns the saved state.
	GroupIterator operator++(int)
	{
		GroupIterator tmp = *this;
		++(*this); // Uses the pre-increment operator to advance the iterator.
		return tmp;
	}

	// Pre-decrement: Moves the iterator backward by one group.
	// Decreases the base iterator by the size of the group and returns a reference to the updated iterator.
	GroupIterator &operator--()
	{
		it_ -= size_;
		return *this;
	}

	// Post-decrement: Moves the iterator backward by one group, but returns the original iterator before the move.
	// Saves the current state, decrements the iterator, and returns the saved state.
	GroupIterator operator--(int)
	{
		GroupIterator tmp = *this;
		--(*this); // Uses the pre-decrement operator to move the iterator backward.
		return tmp;
	}

	// Addition assignment: Moves the iterator forward by a specified number of groups.
	// Multiplies the increment by the group size to advance by the correct number of elements.
	GroupIterator &operator+=(std::size_t increment)
	{
		it_ += size_ * increment;
		return *this;
	}

	// Subtraction assignment: Moves the iterator backward by a specified number of groups.
	// Multiplies the decrement by the group size to move back by the correct number of elements.
	GroupIterator &operator-=(std::size_t increment)
	{
		it_ -= size_ * increment;
		return *this;
	}
};
