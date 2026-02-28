#pragma once

#include "SkipList.hpp"

// ── Destructor ───────────────────────────────────────────────────────────────
template <typename KeyType, typename ValueType>
SkipList<KeyType, ValueType>::~SkipList() {
    SkipListNode<KeyType, ValueType>* cur = front;
    while (cur) {
        SkipListNode<KeyType, ValueType>* tmp = cur->next[0];
        delete cur;
        cur = tmp;
    }
}

// ── SkipListRandomLevel ──────────────────────────────────────────────────────
// Pugh's approach: keep flipping a coin with probability p; count successes
// up to max_level.
template <typename KeyType, typename ValueType>
int SkipListRandomLevel(SkipList<KeyType, ValueType>& list) {
    int level = 0;
    while (list.dist(list.rng) < list.p && level < list.max_level) {
        level++;
    }
    return level;
}

// ── SkipListSearch ───────────────────────────────────────────────────────────
// ❶ Start at front of the topmost level.
// ❷ Two nested loops: outer drops down a level each iteration; inner advances
//    forward while the next node's key < target.
// ❸ After the loops, the target (if present) is at current.next[0].
// ❹ Check existence and key match.
template <typename KeyType, typename ValueType>
std::optional<ValueType> SkipListSearch(SkipList<KeyType, ValueType>& list,
                                        const KeyType& target) {
    int level = list.top_level;
    // ❶
    SkipListNode<KeyType, ValueType>* current = list.front;

    // ❷ Traverse downward and to the right
    while (level >= 0) {
        while (current->next[level] != nullptr &&
               current->next[level]->key < target) {
            current = current->next[level];
        }
        level = level - 1;
    }

    // ❸
    SkipListNode<KeyType, ValueType>* result = current->next[0];

    // ❹
    if (result != nullptr && result->key == target) {
        return result->value;
    }
    return std::nullopt;
}

// ── SkipListInsert ───────────────────────────────────────────────────────────
// ❶ Start at the front, top level.
// ❷ last[] tracks the last node at each level before the insertion point;
//    initialised to list.front.
// ❸ Outer loop: drop level each iteration.
// ❹ Inner loop: advance while next key < key.
// ❺ Record last node at this level.
// ❻ If key already exists, update value and return.
// ❼ Pick a random level for the new node.
// ❽ If new level exceeds top_level, update top_level.
// ❾ Splice the new node into all relevant levels.
template <typename KeyType, typename ValueType>
void SkipListInsert(SkipList<KeyType, ValueType>& list,
                    const KeyType& key, const ValueType& value) {
    int level = list.top_level;
    // ❶
    SkipListNode<KeyType, ValueType>* current = list.front;

    // ❷ Initialise last[] to front for every level
    std::vector<SkipListNode<KeyType, ValueType>*> last(
        list.max_level + 1, list.front);

    // ❸
    while (level >= 0) {
        // ❹
        while (current->next[level] != nullptr &&
               current->next[level]->key < key) {
            current = current->next[level];
        }
        // ❺
        last[level] = current;
        level = level - 1;
    }

    SkipListNode<KeyType, ValueType>* result = current->next[0];

    // ❻ Duplicate key — update value
    if (result != nullptr && result->key == key) {
        result->value = value;
        return;
    }

    // ❼ Pick random level
    int new_level = SkipListRandomLevel(list);

    // ❽ Raise top_level if needed
    if (new_level > list.top_level) {
        list.top_level = new_level;
    }

    // Create the new node
    auto* new_node = new SkipListNode<KeyType, ValueType>(key, value, new_level);

    // ❾ Splice into each level [0..new_level]
    int j = 0;
    while (j <= new_level) {
        new_node->next[j] = last[j]->next[j];
        last[j]->next[j]  = new_node;
        j++;
    }
}

// ── SkipListDelete ───────────────────────────────────────────────────────────
// ❶ Start at the front, top level.
// ❷ Search (same dual loop) while recording last[].
// ❸ If the target is not found, return.
// ❹ Splice the target out of every level it appears in.
// ❺ If we deleted the sole tallest node, lower top_level.
template <typename KeyType, typename ValueType>
void SkipListDelete(SkipList<KeyType, ValueType>& list,
                    const KeyType& target) {
    int level = list.top_level;
    // ❶
    SkipListNode<KeyType, ValueType>* current = list.front;
    std::vector<SkipListNode<KeyType, ValueType>*> last(
        list.max_level + 1, list.front);

    // ❷
    while (level >= 0) {
        while (current->next[level] != nullptr &&
               current->next[level]->key < target) {
            current = current->next[level];
        }
        last[level] = current;
        level = level - 1;
    }

    // ❸
    SkipListNode<KeyType, ValueType>* result = current->next[0];
    if (result == nullptr || result->key != target) {
        return;
    }

    // ❹ Splice out
    level = result->height;
    int j = 0;
    while (j <= level) {
        last[j]->next[j] = result->next[j];
        result->next[j]  = nullptr;
        j++;
    }

    delete result;

    // ❺ Lower top_level if necessary
    if (level == list.top_level) {
        int top = list.top_level;
        while (top > 0 && list.front->next[top] == nullptr) {
            top--;
        }
        list.top_level = top;
    }
}
