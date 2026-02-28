#pragma once
#include <vector>
#include "Max Heap/MaxHeap.hpp"

// Heapsort - Chapter 7: Priority Queues and Heaps
// Sorts an array in decreasing order using a max heap.
// (For increasing order, use a min heap — see HeapsortAsc below.)
//
// Pseudocode:
//   Heapsort(Array: unsorted):
//       Integer: N = length(unsorted)
//       Heap: tmp_heap = Heap of size N
//       Array: result = array of size N
//       Integer: j = 0
//  1   WHILE j < N:
//           HeapInsert(tmp_heap, unsorted[j])
//           j = j + 1
//       j = 0
//  2   WHILE j < N:
//           result[j] = HeapRemoveMax(tmp_heap)
//           j = j + 1
//       return result

template<typename T>
std::vector<T> Heapsort(const std::vector<T>& unsorted);

// Variant that sorts in increasing order using a min heap.
template<typename T>
std::vector<T> HeapsortAsc(const std::vector<T>& unsorted);

#include "Heapsort.tpp"
