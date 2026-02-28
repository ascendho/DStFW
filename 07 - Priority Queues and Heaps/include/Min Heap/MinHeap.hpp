#pragma once
#include <optional>
#include "Heap/Heap.hpp"

// ─── Min Heap operations ────────────────────────────────────────────────────
//
// MinHeapInsert - add a value and restore the min-heap property by bubbling up.
// MinHeapRemoveMin - remove and return the smallest element, restore property.
//
// The only difference from the max heap is the comparison direction:
//   max heap: parent >  child  (bubble up when parent <  child)
//   min heap: parent <  child  (bubble up when parent >  child)

template<typename T>
void MinHeapInsert(Heap<T>& heap, const T& value);

template<typename T>
std::optional<T> MinHeapRemoveMin(Heap<T>& heap);

template<typename T>
bool IsMinEmpty(const Heap<T>& heap) { return heap.last_index == 0; }

template<typename T>
const T& HeapMin(const Heap<T>& heap) { return heap.array[1]; }

#include "MinHeap.tpp"
