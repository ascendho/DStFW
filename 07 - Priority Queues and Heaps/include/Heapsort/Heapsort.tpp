#pragma once
#include "Min Heap/MinHeap.hpp"

// 堆排序 — 降序（最大堆）
template<typename T>
std::vector<T> Heapsort(const std::vector<T>& unsorted) {
    int N = static_cast<int>(unsorted.size());
    // 分配容量为 N+1 的临时堆（1 索引，索引 0 未使用 + N 个有效槽位）
    Heap<T> tmp_heap(N + 1);
    std::vector<T> result(static_cast<std::size_t>(N));

    // 1 构建堆
    int j = 0;
    while (j < N) {
        HeapInsert(tmp_heap, unsorted[static_cast<std::size_t>(j)]);
        j = j + 1;
    }
    // 2 按降序提取
    j = 0;
    while (j < N) {
        result[static_cast<std::size_t>(j)] = *HeapRemoveMax(tmp_heap);
        j = j + 1;
    }
    return result;
}

// HeapsortAsc — 升序（最小堆）
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
