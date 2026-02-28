#pragma once

#include <vector>

// ── Skip List Node ───────────────────────────────────────────────────────────
// Each node stores a key/value pair, a height, and an array of forward
// pointers — one for each level [0, height].  At level L the pointer links to
// the next node that also has height >= L.
template <typename KeyType, typename ValueType>
struct SkipListNode {
    KeyType   key;
    ValueType value;
    int       height;                              // level of this node (0-based)
    std::vector<SkipListNode*> next;               // next[i] → next node at level i

    // Regular data node
    SkipListNode(KeyType key, ValueType value, int height)
        : key(key), value(value), height(height),
          next(height + 1, nullptr) {}

    // Sentinel / front node (no meaningful key/value)
    explicit SkipListNode(int height)
        : key(KeyType{}), value(ValueType{}), height(height),
          next(height + 1, nullptr) {}
};
