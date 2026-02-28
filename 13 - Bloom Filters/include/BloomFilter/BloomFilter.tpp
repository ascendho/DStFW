#pragma once

#include "BloomFilter.hpp"
#include <cmath>
#include <cstdint>

// ── 哈希函数辅助工具 ──────────────────────────────────────────────────────────
// splitmix64 终结器：高质量位混合器，使用不同种子调用时能生成看似独立的哈希值。
static inline uint64_t splitmix64_mix(uint64_t z) {
    z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
    return z ^ (z >> 31);
}

// 为给定大小的布隆过滤器生成第 i 个哈希函数。
// 使用双重哈希：
//   h_i(key) = (h1(key) + i · h2(key)) % size
// 其中 h1 和 h2 由 std::hash<KeyType> 与不同种子混合得到。
template <typename KeyType>
static std::function<size_t(const KeyType&)>
make_hash_fn(int i, int size) {
    const uint64_t seed_a = splitmix64_mix(static_cast<uint64_t>(i) * 2654435761ULL + 1);
    const uint64_t seed_b = splitmix64_mix(static_cast<uint64_t>(i) * 2246822519ULL + 2);
    return [seed_a, seed_b, size](const KeyType& key) -> size_t {
        uint64_t raw = static_cast<uint64_t>(std::hash<KeyType>{}(key));
        uint64_t h1  = splitmix64_mix(raw ^ seed_a);
        uint64_t h2  = splitmix64_mix(raw ^ seed_b) | 1ULL; // 保持奇数 → 完整周期
        return static_cast<size_t>((h1 + h2) % static_cast<uint64_t>(size));
    };
}

// ── 构造函数 ──────────────────────────────────────────────────────────────────
template <typename KeyType>
BloomFilter<KeyType>::BloomFilter(int size, int k)
    : size(size), k(k), bins(size, false) {
    for (int i = 0; i < k; i++) {
        h.push_back(make_hash_fn<KeyType>(i, size));
    }
}

template <typename KeyType>
BloomFilter<KeyType>::BloomFilter(
    int size, int k,
    std::vector<std::function<size_t(const KeyType&)>> hash_fns)
    : size(size), k(k), bins(size, false), h(std::move(hash_fns)) {}

// ── BloomFilterInsertKey ──────────────────────────────────────────────────────
// 对 k 个哈希函数中的每一个，将键映射到索引并设置 bins[index] = 1。
template <typename KeyType>
void BloomFilterInsertKey(BloomFilter<KeyType>& filter, const KeyType& key) {
    int i = 0;
    while (i < filter.k) {
        size_t index = filter.h[i](key);
        filter.bins[index] = true;
        i = i + 1;
    }
}

// ── BloomFilterLookup ─────────────────────────────────────────────────────────
// 对 k 个哈希函数中的每一个，检查 bins[index]。
// 发现 0 时立即返回 false（确定未命中）。
// 仅当所有 k 个槽位均为 1 时返回 true（可能命中）。
template <typename KeyType>
bool BloomFilterLookup(const BloomFilter<KeyType>& filter, const KeyType& key) {
    int i = 0;
    while (i < filter.k) {
        size_t index = filter.h[i](key);
        if (!filter.bins[index]) {
            return false;
        }
        i = i + 1;
    }
    return true;
}

// ── BloomFilterFalsePositiveRate ──────────────────────────────────────────────
// FPR ≈ (1 − (1 − 1/m)^(n·k))^k
inline double BloomFilterFalsePositiveRate(int n, int m, int k) {
    double inner = std::pow(1.0 - 1.0 / m, static_cast<double>(n) * k);
    return std::pow(1.0 - inner, k);
}
