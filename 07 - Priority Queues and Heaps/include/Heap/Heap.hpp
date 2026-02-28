#pragma once
#include <vector>

// 堆 — 第七章：优先队列与堆
// 基于数组的堆（1 索引：array[1..last_index] 为有效元素；
// array[0] 未使用）。最大堆和最小堆共享此结构。
//
// Pseudocode:
//   Heap {
//       Array: array
//       Integer: array_size
//       Integer: last_index
//   }
template<typename T>
struct Heap {
    std::vector<T> array;      // 物理存储；索引 0 未使用
    int array_size;            // 数组的物理大小
    int last_index;            // 最后一个元素的索引；0 表示空

    explicit Heap(int initial_capacity = 16)
        : array(static_cast<std::size_t>(initial_capacity)),
          array_size(initial_capacity),
          last_index(0) {}
};
