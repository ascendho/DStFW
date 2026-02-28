#pragma once

#include "SkipListNode.hpp"
#include <optional>
#include <random>

// ── Skip List ────────────────────────────────────────────────────────────────
// A probabilistic sorted linked list with multi-level forward pointers.
//   top_level  – highest level currently in use (0-based)
//   max_level  – highest allowable level (inclusive)
//   front      – dummy sentinel node (height == max_level); stores the
//                head pointers for every level
template <typename KeyType, typename ValueType>
struct SkipList {
    int                                  top_level;
    int                                  max_level;
    SkipListNode<KeyType, ValueType>*    front;

    // Probability of promoting a node one additional level (default 0.5).
    double p;

    // RNG state
    std::mt19937                         rng;
    std::uniform_real_distribution<>     dist;

    explicit SkipList(int max_level, double p = 0.5,
                      unsigned seed = std::random_device{}())
        : top_level(0), max_level(max_level),
          front(new SkipListNode<KeyType, ValueType>(max_level)),
          p(p), rng(seed), dist(0.0, 1.0) {}

    ~SkipList();
};

// ── Operations ───────────────────────────────────────────────────────────────
template <typename KeyType, typename ValueType>
std::optional<ValueType> SkipListSearch(SkipList<KeyType, ValueType>& list,
                                        const KeyType& target);

template <typename KeyType, typename ValueType>
void SkipListInsert(SkipList<KeyType, ValueType>& list,
                    const KeyType& key, const ValueType& value);

template <typename KeyType, typename ValueType>
void SkipListDelete(SkipList<KeyType, ValueType>& list,
                    const KeyType& target);

// ── Helper: pick a random level ──────────────────────────────────────────────
template <typename KeyType, typename ValueType>
int SkipListRandomLevel(SkipList<KeyType, ValueType>& list);

#include "SkipList.tpp"
