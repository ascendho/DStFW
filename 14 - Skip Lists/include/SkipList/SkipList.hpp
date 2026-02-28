#pragma once

#include "SkipListNode.hpp"
#include <optional>
#include <random>

// ── 跳表 ──────────────────────────────────────────────────────────────────────
// 一种基于概率的有序链表，具有多层前向指针。
//   top_level  – 当前使用的最高层级（从 0 开始）
//   max_level  – 允许的最高层级（含）
//   front      – 虚拟哨兵节点（height == max_level）；存储每个层级的
//                头指针
template <typename KeyType, typename ValueType>
struct SkipList {
    int                                  top_level;
    int                                  max_level;
    SkipListNode<KeyType, ValueType>*    front;

    // 节点提升一个额外层级的概率（默认 0.5）。
    double p;

    // 随机数生成器状态
    std::mt19937                         rng;
    std::uniform_real_distribution<>     dist;

    explicit SkipList(int max_level, double p = 0.5,
                      unsigned seed = std::random_device{}())
        : top_level(0), max_level(max_level),
          front(new SkipListNode<KeyType, ValueType>(max_level)),
          p(p), rng(seed), dist(0.0, 1.0) {}

    ~SkipList();
};

// ── 操作 ──────────────────────────────────────────────────────────────────────
template <typename KeyType, typename ValueType>
std::optional<ValueType> SkipListSearch(SkipList<KeyType, ValueType>& list,
                                        const KeyType& target);

template <typename KeyType, typename ValueType>
void SkipListInsert(SkipList<KeyType, ValueType>& list,
                    const KeyType& key, const ValueType& value);

template <typename KeyType, typename ValueType>
void SkipListDelete(SkipList<KeyType, ValueType>& list,
                    const KeyType& target);

// ── 辅助函数：选取随机层级 ────────────────────────────────────────────────────
template <typename KeyType, typename ValueType>
int SkipListRandomLevel(SkipList<KeyType, ValueType>& list);

#include "SkipList.tpp"
