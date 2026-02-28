#pragma once

#include <vector>
#include <functional>
#include <cstddef>

// ── Data Structure ────────────────────────────────────────────────────────────
// A Bloom filter stores a bit array of `size` bins and k independent hash
// functions.  An inserted key flips k bins to 1; a lookup returns true only if
// all k bins are 1 (no false negatives; occasional false positives possible).
//
// Hash-function family: uses the splitmix64 finalizer seeded differently for
// each of the k slots, giving independent-looking mappings from a single call
// to std::hash<KeyType>.
template <typename KeyType>
struct BloomFilter {
    int size;                                              // m – number of bins
    int k;                                                 // number of hash fns
    std::vector<bool> bins;                                // bit array
    std::vector<std::function<size_t(const KeyType&)>> h; // hash functions

    // Construct with given size/k; hash functions are seeded automatically.
    BloomFilter(int size, int k);

    // Construct with an explicitly supplied hash-function family (size k).
    BloomFilter(int size, int k,
                std::vector<std::function<size_t(const KeyType&)>> hash_fns);
};

// ── Operations ────────────────────────────────────────────────────────────────
template <typename KeyType>
void BloomFilterInsertKey(BloomFilter<KeyType>& filter, const KeyType& key);

// Returns false  → key is definitely not present (no false negatives).
// Returns true   → key is probably present (false positive possible).
template <typename KeyType>
bool BloomFilterLookup(const BloomFilter<KeyType>& filter, const KeyType& key);

// ── Formula ───────────────────────────────────────────────────────────────────
// Theoretical false-positive rate for n items, m bins, k hash functions:
//   FPR ≈ (1 − (1 − 1/m)^(n·k))^k
double BloomFilterFalsePositiveRate(int n, int m, int k);

#include "BloomFilter.tpp"
