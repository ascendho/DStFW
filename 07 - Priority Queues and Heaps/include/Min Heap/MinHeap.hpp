#pragma once
#include <optional>
#include "Heap/Heap.hpp"

// ─── 最小堆操作 ─────────────────────────────────────────────────────────────
//
// MinHeapInsert — 插入一个值，并通过上浮恢复最小堆性质。
// MinHeapRemoveMin — 移除并返回最小元素，恢复堆性质。
//
// 与最大堆的唯一区别在于比较方向：
//   最大堆：父节点 > 子节点（当父节点 < 子节点时上浮）
//   最小堆：父节点 < 子节点（当父节点 > 子节点时上浮）

template<typename T>
void MinHeapInsert(Heap<T>& heap, const T& value);

template<typename T>
std::optional<T> MinHeapRemoveMin(Heap<T>& heap);

template<typename T>
bool IsMinEmpty(const Heap<T>& heap) { return heap.last_index == 0; }

template<typename T>
const T& HeapMin(const Heap<T>& heap) { return heap.array[1]; }

#include "MinHeap.tpp"
