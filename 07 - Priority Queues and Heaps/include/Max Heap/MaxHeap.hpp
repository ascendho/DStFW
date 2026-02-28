#pragma once
#include <optional>
#include "Heap/Heap.hpp"

// ─── 最大堆操作 ─────────────────────────────────────────────────────────────
//
// HeapInsert — 插入一个值，并通过上浮恢复最大堆性质。
// HeapRemoveMax — 移除并返回最大元素，通过将提升的最后一个元素
//                 下沉来恢复堆性质。
// UpdateMaxValue — 更新给定 1 索引位置的值并修复堆。

template<typename T>
void HeapInsert(Heap<T>& heap, const T& value);

template<typename T>
std::optional<T> HeapRemoveMax(Heap<T>& heap);

template<typename T>
void UpdateMaxValue(Heap<T>& heap, int index, const T& value);

// 便捷辅助函数
template<typename T>
bool IsEmpty(const Heap<T>& heap) { return heap.last_index == 0; }

template<typename T>
const T& HeapMax(const Heap<T>& heap) { return heap.array[1]; }

#include "MaxHeap.tpp"
