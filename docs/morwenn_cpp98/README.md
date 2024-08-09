# Translating Morwenn's code in C++98

### 1. Starting building the Main Chain

The main chain is built by taking pairs of sorted elements and adding them to the `mainChain` list. In C++11, `std::next` is used to get the iterator pointing to the next element. In C++98, since `std::next` isn't available, we use `std::advance` to move the iterator forward.

#### Original Code

```cpp
std::list<RandomAccessIterator> chain = {first, std::next(first)};
```

Here, `first` is added to the `chain` (main chain), and then `std::next(first)` is added, which gets the iterator to the next element after `first`.

#### Translation to C++98

We renames `chain` into `mainChain`.

```cpp
mainChain.push_back(first);
RandomAccessIterator second = first;
std::advance(second, 1); // equivalent to std::next(first) in C++11
mainChain.push_back(second);
```

In this translation:

- `first` is added to the `mainChain`.
- We assigne to `second` the value of `first`.
- `second` is advanced by 1 (using `std::advance`) and then added to the `mainChain`.

### 2. Role of `next` in `PendChainNode`

In the original code, `next` (which we are considering renaming to `mainChainPosition`) is a pointer to the corresponding position in the `mainChain`.

#### Purpose of `next` (or `mainChainPosition`)

The `next` pointer in `PendChainNode` is used to know where in the `mainChain` a pending element should be inserted. This isn't about the next element in the `pendChain` itself, but rather its corresponding place in the `mainChain`.

### 3. Example of `next` Usage

In the following code snippet from the original code:

```cpp
for (auto it = first + 2; it != end; it += 2)
{
    auto tmp = chain.insert(chain.end(), std::next(it));
    pend.push_back({it, tmp});
}
```

Here’s what’s happening:

- Elements are being inserted into the `mainChain`.
- For each `it` in the iteration (which represents a sorted element in a pair), the next element is found using `std::next(it)` and added to `mainChain`.
- The corresponding position (`tmp`) in the `mainChain` is stored in the `pendChain` for later use.

### Conclusion

- **`std::next` vs. `advance`:** Yes, `std::advance(second, 1)` is a direct translation of `std::next(first)` in C++98, and this part of the code is correctly understood and translated.
- **`next` in `PendChainNode`:** This pointer refers to the place in the `mainChain` where the pending element should eventually be inserted, not to the next element in the `pendChain`. It’s correctly serving as a reference to the appropriate place in the `mainChain`.

The process described in your question is correct. The `std::next` in the original code simply points to the next element in the sequence, and the translation using `std::advance` is appropriate. The `next` field in `PendChainNode` (which we might rename to something more descriptive like `mainChainPosition`) is indeed intended to point to the paired element in the `mainChain`. This understanding is key to properly implementing and debugging this part of the algorithm.
