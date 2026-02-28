#pragma once

#include "BloomFilter.hpp"
#include <cmath>
#include <cstdint>

// ── Hash-function helper ──────────────────────────────────────────────────────
// splitmix64 finalizer: high-quality bit mixer, makes independent-looking hashes
// when called with different seeds.
static inline uint64_t splitmix64_mix(uint64_t z) {
    z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
    return z ^ (z >> 31);
}

// Make the i-th hash function for a BloomFilter of given size.
// Uses double hashing:
//   h_i(key) = (h1(key) + i · h2(key)) % size
// where h1 and h2 are derived from std::hash<KeyType> mixed with distinct seeds.
template <typename KeyType>
static std::function<size_t(const KeyType&)>
make_hash_fn(int i, int size) {
    const uint64_t seed_a = splitmix64_mix(static_cast<uint64_t>(i) * 2654435761ULL + 1);
    const uint64_t seed_b = splitmix64_mix(static_cast<uint64_t>(i) * 2246822519ULL + 2);
    return [seed_a, seed_b, size](const KeyType& key) -> size_t {
        uint64_t raw = static_cast<uint64_t>(std::hash<KeyType>{}(key));
        uint64_t h1  = splitmix64_mix(raw ^ seed_a);
        uint64_t h2  = splitmix64_mix(raw ^ seed_b) | 1ULL; // keep odd → full period
        return static_cast<size_t>((h1 + h2) % static_cast<uint64_t>(size));
    };
}

// ── Constructors ──────────────────────────────────────────────────────────────
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
// For each of the k hash functions, map key to an index and set bins[index] = 1.
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
// For each of the k hash functions, check bins[index].
// Return false immediately upon finding a 0 (definite miss).
// Return true only if all k bins are 1 (probable hit).
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
