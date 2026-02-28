#pragma once

#include <vector>

// ── 跳表节点 ──────────────────────────────────────────────────────────────────
// 每个节点存储一个键/值对、一个高度和一组前向指针 ── 每个层级
// [0, height] 各一个。在层级 L，指针指向下一个高度 >= L 的节点。
template <typename KeyType, typename ValueType>
struct SkipListNode {
    KeyType   key;
    ValueType value;
    int       height;                              // 该节点的层级（从 0 开始）
    std::vector<SkipListNode*> next;               // next[i] → 第 i 层的下一个节点

    // 常规数据节点
    SkipListNode(KeyType key, ValueType value, int height)
        : key(key), value(value), height(height),
          next(height + 1, nullptr) {}

    // 哨兵/头节点（无实际键/值）
    explicit SkipListNode(int height)
        : key(KeyType{}), value(ValueType{}), height(height),
          next(height + 1, nullptr) {}
};
