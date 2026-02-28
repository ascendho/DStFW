#pragma once
#include <optional>
#include "Heap/Heap.hpp"

// ─── Max Heap operations ────────────────────────────────────────────────────
//
// HeapInsert - add a value and restore the max-heap property by bubbling up.
// HeapRemoveMax - remove and return the max element, restore heap property by
//                 bubbling the promoted last element down.
// UpdateMaxValue - update the value at a given 1-based index and fix the heap.

template<typename T>
void HeapInsert(Heap<T>& heap, const T& value);

template<typename T>
std::optional<T> HeapRemoveMax(Heap<T>& heap);

template<typename T>
void UpdateMaxValue(Heap<T>& heap, int index, const T& value);

// Convenience helpers
template<typename T>
bool IsEmpty(const Heap<T>& heap) { return heap.last_index == 0; }

template<typename T>
const T& HeapMax(const Heap<T>& heap) { return heap.array[1]; }

#include "MaxHeap.tpp"
