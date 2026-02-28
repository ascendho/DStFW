#pragma once
#include <vector>

// Heap - Chapter 7: Priority Queues and Heaps
// Array-based heap (1-indexed: array[1..last_index] are the valid elements;
// array[0] is unused). Both max and min heaps share this structure.
//
// Pseudocode:
//   Heap {
//       Array: array
//       Integer: array_size
//       Integer: last_index
//   }
template<typename T>
struct Heap {
    std::vector<T> array;      // physical storage; index 0 unused
    int array_size;            // physical size of array
    int last_index;            // index of the last element; 0 means empty

    explicit Heap(int initial_capacity = 16)
        : array(static_cast<std::size_t>(initial_capacity)),
          array_size(initial_capacity),
          last_index(0) {}
};
