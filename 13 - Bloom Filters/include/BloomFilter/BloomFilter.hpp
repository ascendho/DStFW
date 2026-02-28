#pragma once

#include <vector>
#include <functional>
#include <cstddef>

// ── 数据结构 ──────────────────────────────────────────────────────────────────
// 布隆过滤器存储一个包含 `size` 个槽位的位数组和 k 个独立的哈希函数。
// 插入的键将 k 个槽位翻转为 1；查找仅在所有 k 个槽位均为 1 时返回 true
//（无假阴性；可能出现假阳性）。
//
// 哈希函数族：使用 splitmix64 终结器，为 k 个槽位分别设置不同的种子，
// 从单次 std::hash<KeyType> 调用产生看似独立的映射。
template <typename KeyType>
struct BloomFilter {
    int size;                                              // m – 槽位数量
    int k;                                                 // 哈希函数数量
    std::vector<bool> bins;                                // 位数组
    std::vector<std::function<size_t(const KeyType&)>> h; // 哈希函数

    // 使用给定的 size/k 构造；哈希函数自动设置种子。
    BloomFilter(int size, int k);

    // 使用显式提供的哈希函数族（大小为 k）构造。
    BloomFilter(int size, int k,
                std::vector<std::function<size_t(const KeyType&)>> hash_fns);
};

// ── 操作 ──────────────────────────────────────────────────────────────────────
template <typename KeyType>
void BloomFilterInsertKey(BloomFilter<KeyType>& filter, const KeyType& key);

// 返回 false → 键一定不存在（无假阴性）。
// 返回 true  → 键可能存在（可能为假阳性）。
template <typename KeyType>
bool BloomFilterLookup(const BloomFilter<KeyType>& filter, const KeyType& key);

// ── 公式 ──────────────────────────────────────────────────────────────────────
// n 个元素、m 个槽位、k 个哈希函数的理论假阳性率：
//   FPR ≈ (1 − (1 − 1/m)^(n·k))^k
double BloomFilterFalsePositiveRate(int n, int m, int k);

#include "BloomFilter.tpp"
