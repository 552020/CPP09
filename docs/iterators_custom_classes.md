<!-- https://chatgpt.com/c/6702947a-9d07-4e21-9845-efbb3b2fff19 -->

In C++98, the concept of iterator adaptors was less extensive compared to later C++ standards, but there were still some common patterns where similar behavior could be seen. Below are examples of iterator-like classes in C++98 or standard C++ that exhibit similar patterns:

### 1. **`std::reverse_iterator` in C++98:**

Even in C++98, the `std::reverse_iterator` was available and provided the `base()` method. This is the most commonly used example from that era.

#### Example:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec;
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i);
    }

    std::vector<int>::reverse_iterator rit = vec.rbegin();

    // Access the base iterator
    std::vector<int>::iterator it = rit.base();

    std::cout << "Reverse iterator points to: " << *rit << std::endl; // Outputs: 5
    std::cout << "Base iterator points to: " << *it << std::endl;     // Outputs: 4

    return 0;
}
```

- **Explanation:** In this example, `std::reverse_iterator` is used in C++98 to iterate through a vector in reverse. The `base()` method returns the corresponding forward iterator that is one position ahead of the element currently pointed to by the reverse iterator.

### 2. **Custom Iterator Adaptors (C++98):**

In C++98, it was common to implement custom iterator adaptors for specific needs, which could also include a `base()` method. For example, a custom filtering iterator that iterates over only certain elements of a container.

#### Example of a Custom Filtering Iterator:

```cpp
#include <iostream>
#include <vector>
#include <iterator>

template <typename Iterator, typename Predicate>
class FilteringIterator {
public:
    typedef typename std::iterator_traits<Iterator>::value_type value_type;

    FilteringIterator(Iterator it, Iterator end, Predicate pred)
        : it_(it), end_(end), pred_(pred) {
        while (it_ != end_ && !pred_(*it_)) {
            ++it_;
        }
    }

    value_type operator*() const {
        return *it_;
    }

    FilteringIterator& operator++() {
        do {
            ++it_;
        } while (it_ != end_ && !pred_(*it_));
        return *this;
    }

    bool operator!=(const FilteringIterator& other) const {
        return it_ != other.it_;
    }

    Iterator base() const {
        return it_;
    }

private:
    Iterator it_;
    Iterator end_;
    Predicate pred_;
};

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    auto is_even = [](int value) { return value % 2 == 0; };

    FilteringIterator<std::vector<int>::iterator, decltype(is_even)>
        it(vec.begin(), vec.end(), is_even);

    for (; it != FilteringIterator<std::vector<int>::iterator, decltype(is_even)>(vec.end(), vec.end(), is_even); ++it) {
        std::cout << *it << " ";  // Outputs: 2 4 6
    }

    return 0;
}
```

- **Explanation:** This custom `FilteringIterator` skips elements that don't match a certain predicate (in this case, it filters out odd numbers). The `base()` method returns the current position of the underlying iterator. This kind of custom iterator is an example of how `base()` might be used in custom iterator adaptors.

### 3. **Custom Reverse Iterator (C++98):**

If the standard `reverse_iterator` didn't exist (as in some limited environments), you might implement your own version that would likely include a `base()` method.

#### Simplified Custom Reverse Iterator:

```cpp
#include <iostream>
#include <vector>

template <typename Iterator>
class ReverseIterator {
public:
    typedef Iterator iterator_type;

    ReverseIterator(Iterator it) : it_(it) {}

    iterator_type base() const {
        return it_;
    }

    typename std::iterator_traits<Iterator>::reference operator*() const {
        Iterator tmp = it_;
        return *--tmp;
    }

    ReverseIterator& operator++() {
        --it_;
        return *this;
    }

    bool operator!=(const ReverseIterator& other) const {
        return it_ != other.it_;
    }

private:
    Iterator it_;
};

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    ReverseIterator<std::vector<int>::iterator> rit(vec.end());

    while (rit != ReverseIterator<std::vector<int>::iterator>(vec.begin())) {
        std::cout << *rit << " ";  // Outputs: 5 4 3 2 1
        ++rit;
    }

    return 0;
}
```

- **Explanation:** This example shows a simplified custom reverse iterator implementation, which includes a `base()` method similar to `std::reverse_iterator`. The `base()` method gives access to the underlying iterator, providing the standard interface you'd expect.

### Summary:

In C++98, while the standard library was less rich in iterator adaptors compared to later standards, you still had `std::reverse_iterator`, which featured the `base()` method. Additionally, custom iterator adaptors could be written to include similar functionality, such as filtering or reversing, often implementing a `base()` method for accessing the underlying iterator.
