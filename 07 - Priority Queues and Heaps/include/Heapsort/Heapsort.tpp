#pragma once
#include "Min Heap/MinHeap.hpp"

// Heapsort — decreasing order (max heap)
template<typename T>
std::vector<T> Heapsort(const std::vector<T>& unsorted) {
    int N = static_cast<int>(unsorted.size());
    // Allocate tmp_heap with capacity N+1 (1-based, so index 0 unused + N valid slots)
    Heap<T> tmp_heap(N + 1);
    std::vector<T> result(static_cast<std::size_t>(N));

    // 1 Build heap
    int j = 0;
    while (j < N) {
        HeapInsert(tmp_heap, unsorted[static_cast<std::size_t>(j)]);
        j = j + 1;
    }
    // 2 Extract in decreasing order
    j = 0;
    while (j < N) {
        result[static_cast<std::size_t>(j)] = *HeapRemoveMax(tmp_heap);
        j = j + 1;
    }
    return result;
}

// HeapsortAsc — increasing order (min heap)
template<typename T>
std::vector<T> HeapsortAsc(const std::vector<T>& unsorted) {
    int N = static_cast<int>(unsorted.size());
    Heap<T> tmp_heap(N + 1);
    std::vector<T> result(static_cast<std::size_t>(N));

    int j = 0;
    while (j < N) {
        MinHeapInsert(tmp_heap, unsorted[static_cast<std::size_t>(j)]);
        j = j + 1;
    }
    j = 0;
    while (j < N) {
        result[static_cast<std::size_t>(j)] = *MinHeapRemoveMin(tmp_heap);
        j = j + 1;
    }
    return result;
}
