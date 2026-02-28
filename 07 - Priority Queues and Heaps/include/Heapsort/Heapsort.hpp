#pragma once
#include <vector>
#include "Max Heap/MaxHeap.hpp"

// 堆排序 — 第七章：优先队列与堆
// 使用最大堆将数组按降序排序。
// （若需升序，使用最小堆 — 参见下方的 HeapsortAsc。）
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

// 使用最小堆按升序排序的变体。
template<typename T>
std::vector<T> HeapsortAsc(const std::vector<T>& unsorted);

#include "Heapsort.tpp"
