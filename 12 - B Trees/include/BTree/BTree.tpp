#pragma once

#include "BTree.hpp"
#include <stdexcept>

// ── BTreeNodeSearch ──────────────────────────────────────────────────────────
// ❶ Scan across keys; if we find a match, return it.
// ❸ If we reach a leaf without a match, the key is absent.
// ❹ Otherwise recurse into the appropriate child.
template <typename KeyType>
std::optional<KeyType> BTreeNodeSearch(BTreeNode<KeyType>* node, KeyType target) {
    int i = 0;
    // ❶ Advance i while target >= keys[i]
    while (i < node->size && target >= node->keys[i]) {
        // ❷ Exact match
        if (target == node->keys[i]) {
            return node->keys[i];
        }
        i++;
    }
    // ❸ Hit a leaf without finding the key
    if (node->is_leaf) {
        return std::nullopt;
    }
    // ❹ Descend into child[i]
    return BTreeNodeSearch(node->children[i], target);
}

template <typename KeyType>
std::optional<KeyType> BTreeSearch(BTree<KeyType>* tree, KeyType target) {
    return BTreeNodeSearch(tree->root, target);
}

// ── BTreeNodeAddKey ──────────────────────────────────────────────────────────
// ❶ Shift keys (and right-side children) right until the correct slot.
// ❷ Place the new key and next_child pointer.
// ❸ Increment size.
template <typename KeyType>
void BTreeNodeAddKey(BTreeNode<KeyType>* node, KeyType key,
                     BTreeNode<KeyType>* next_child) {
    // ❶ Shift right from end
    int i = node->size - 1;
    while (i >= 0 && key < node->keys[i]) {
        node->keys[i + 1]     = node->keys[i];
        node->children[i + 2] = node->children[i + 1];
        i--;
    }
    // ❷ Insert
    node->keys[i + 1] = key;
    if (!node->is_leaf) {
        node->children[i + 2] = next_child;
    }
    // ❸ Update size
    node->size++;
}

// ── BTreeNodeIsOverFull ──────────────────────────────────────────────────────
template <typename KeyType>
bool BTreeNodeIsOverFull(BTreeNode<KeyType>* node) {
    return node->size == 2 * node->k + 1;
}

// ── BTreeNodeSplit ───────────────────────────────────────────────────────────
// 将 node->children[child_index]（old_child）分割为两个节点。
// ❶ Identify old_child; allocate new_child with same leaf status.
// ❷ split_index = floor(old_child.size / 2); split_key = keys[split_index].
// ❸ Copy the upper half of old_child into new_child.
// ❹ If internal, copy the last child pointer to new_child.
// ❺ Null out the promoted key slot.
// ❻ Insert split_key (with new_child as next pointer) into the parent.
// ❼ Adjust sizes.
template <typename KeyType>
void BTreeNodeSplit(BTreeNode<KeyType>* node, int child_index) {
    // ❶
    BTreeNode<KeyType>* old_child = node->children[child_index];
    BTreeNode<KeyType>* new_child = new BTreeNode<KeyType>(old_child->k, old_child->is_leaf);

    // ❷
    int split_index = old_child->size / 2;
    KeyType split_key = old_child->keys[split_index];

    // ❸ Copy upper half (after split_index) to new_child
    int old_index = split_index + 1;
    int new_index = 0;
    while (old_index < old_child->size) {
        new_child->keys[new_index] = old_child->keys[old_index];
        if (!old_child->is_leaf) {
            new_child->children[new_index] = old_child->children[old_index];
        }
        old_child->keys[old_index] = KeyType{};
        if (!old_child->is_leaf) {
            old_child->children[old_index] = nullptr;
        }
        old_index++;
        new_index++;
    }

    // ❹ Copy last child pointer if internal
    if (!old_child->is_leaf) {
        new_child->children[new_index] = old_child->children[old_child->size];
        old_child->children[old_child->size] = nullptr;
    }

    // ❺ Null out the promoted split key
    old_child->keys[split_index] = KeyType{};

    // ❻ Add split_key to parent, linking new_child as its right child
    BTreeNodeAddKey(node, split_key, new_child);

    // ❼ Update sizes
    new_child->size = old_child->size - split_index - 1;
    old_child->size = split_index;
}

// ── BTreeNodeInsert ──────────────────────────────────────────────────────────
// ❶ Scan to find the insertion position i.
// ❷ If exact match, key already present (no-op or update).
// ❸ Leaf: directly add key.
// ❹ Internal: recurse then ❺❻ split if overfull.
template <typename KeyType>
void BTreeNodeInsert(BTreeNode<KeyType>* node, KeyType key) {
    // ❶
    int i = 0;
    while (i < node->size && key >= node->keys[i]) {
        // ❷ Duplicate — already present, do nothing
        if (key == node->keys[i]) {
            return;
        }
        i++;
    }

    if (node->is_leaf) {
        // ❸
        BTreeNodeAddKey(node, key, static_cast<BTreeNode<KeyType>*>(nullptr));
    } else {
        // ❹ Recurse
        BTreeNodeInsert(node->children[i], key);
        // ❺❻ Repair overfull child
        if (BTreeNodeIsOverFull(node->children[i])) {
            BTreeNodeSplit(node, i);
        }
    }
}

// ── BTreeInsert ──────────────────────────────────────────────────────────────
// ❶ Delegate to recursive insert.
// ❷ If root itself overflowed, create a new root and split the old one.
template <typename KeyType>
void BTreeInsert(BTree<KeyType>* tree, KeyType key) {
    // ❶
    BTreeNodeInsert(tree->root, key);
    // ❷
    if (BTreeNodeIsOverFull(tree->root)) {
        // ❸ Allocate fresh internal root
        BTreeNode<KeyType>* new_root = new BTreeNode<KeyType>(tree->k, false);
        new_root->size = 0;
        // ❹ Old root becomes children[0]
        new_root->children[0] = tree->root;
        // ❺ Split the old root
        BTreeNodeSplit(new_root, 0);
        // ❻ Update tree's root
        tree->root = new_root;
    }
}

// ── BTreeNodeIsUnderFull ────────────────────────────────────────────────────
template <typename KeyType>
bool BTreeNodeIsUnderFull(BTreeNode<KeyType>* node) {
    return node->size < node->k;
}

// ── BTreeNodeMerge ───────────────────────────────────────────────────────────
// 将 node->children[index+1] 合并到 node->children[index] 中，
// 使用 node->keys[index] 作为分隔键。
// ❶ Retrieve childL and childR.
// ❷ Append parent separating key and childR's first child pointer to childL.
// ❸ Copy all of childR's keys/children into childL.
// ❹ Remove the separating key and childR pointer from the parent.
template <typename KeyType>
void BTreeNodeMerge(BTreeNode<KeyType>* node, int index) {
    // ❶
    BTreeNode<KeyType>* childL = node->children[index];
    BTreeNode<KeyType>* childR = node->children[index + 1];

    // ❷ Append separating key from parent and first child of childR
    int loc = childL->size;
    childL->keys[loc] = node->keys[index];
    if (!childL->is_leaf) {
        childL->children[loc + 1] = childR->children[0];
    }
    loc = loc + 1;

    // ❸ Copy childR's keys and children
    int i = 0;
    while (i < childR->size) {
        childL->keys[loc + i] = childR->keys[i];
        if (!childL->is_leaf) {
            childL->children[loc + i + 1] = childR->children[i + 1];
        }
        i++;
    }
    childL->size = childL->size + childR->size + 1;

    // ❹ Remove the parent's separating key and pointer to childR
    i = index;
    while (i < node->size - 1) {
        node->keys[i]         = node->keys[i + 1];
        node->children[i + 1] = node->children[i + 2];
        i++;
    }
    node->keys[i]         = KeyType{};
    node->children[i + 1] = nullptr;
    node->size             = node->size - 1;

    // 释放已清空的右子节点
    delete childR;
}

// ── BTreeNodeTransferLeft ───────────────────────────────────────────────────
// 通过父节点在 index 处的分隔键，从右子节点向左子节点转移一个键。
// ❶ Retrieve children and middle_key.
// ❷ Replace parent separator with childR->keys[0].
// ❸ Append middle_key (and first-child ptr) to the end of childL.
// ❹ Shift childR keys/children left.
// ❺ Null out the vacated slots.
// ❻ Decrement childR->size.
template <typename KeyType>
void BTreeNodeTransferLeft(BTreeNode<KeyType>* node, int index) {
    // ❶
    BTreeNode<KeyType>* childL = node->children[index];
    BTreeNode<KeyType>* childR = node->children[index + 1];
    KeyType middle_key = node->keys[index];

    // ❷ Move childR's first key up to parent
    node->keys[index] = childR->keys[0];

    // ❸ Append parent's key to childL
    childL->keys[childL->size] = middle_key;
    if (!childR->is_leaf) {
        childL->children[childL->size + 1] = childR->children[0];
    }
    childL->size = childL->size + 1;

    // ❹ Shift childR left
    int i = 0;
    while (i < childR->size - 1) {
        childR->keys[i] = childR->keys[i + 1];
        if (!childR->is_leaf) {
            childR->children[i] = childR->children[i + 1];
        }
        i++;
    }

    // ❺ Null vacated slots
    childR->keys[i] = KeyType{};
    if (!childR->is_leaf) {
        childR->children[i]     = childR->children[i + 1];
        childR->children[i + 1] = nullptr;
    }

    // ❻
    childR->size = childR->size - 1;
}

// ── BTreeNodeTransferRight ──────────────────────────────────────────────────
// 通过父节点在 index 处的分隔键，从左子节点向右子节点转移一个键。
// ❶ Retrieve children and middle_key.
// ❷ Shift childR right to make room.
// ❸ Prepend middle_key (and childL's last child ptr) to childR.
// ❹ Move childL's last key to parent.
// ❺ Null vacated slots and decrement childL->size.
template <typename KeyType>
void BTreeNodeTransferRight(BTreeNode<KeyType>* node, int index) {
    // ❶
    BTreeNode<KeyType>* childL = node->children[index];
    BTreeNode<KeyType>* childR = node->children[index + 1];
    KeyType middle_key = node->keys[index];

    // ❷ Shift childR right
    int i = childR->size - 1;
    while (i >= 0) {
        childR->keys[i + 1] = childR->keys[i];
        if (!childR->is_leaf) {
            childR->children[i + 2] = childR->children[i + 1];
        }
        i--;
    }
    if (!childR->is_leaf) {
        childR->children[1] = childR->children[0];
    }

    // ❸ Prepend middle_key and childL's last child
    childR->keys[0] = middle_key;
    if (!childR->is_leaf) {
        childR->children[0] = childL->children[childL->size];
    }
    childR->size = childR->size + 1;

    // ❹ Move childL's last key to parent
    node->keys[index] = childL->keys[childL->size - 1];

    // ❺ Clean up childL
    childL->keys[childL->size - 1] = KeyType{};
    if (!childL->is_leaf) {
        childL->children[childL->size] = nullptr;
    }
    childL->size = childL->size - 1;
}

// ── BTreeNodeRepairUnderFull ─────────────────────────────────────────────────
// ❶ Adjust child index so we always work with (child, child+1) pair.
// ❷ Compute total keys in the two siblings.
// ❸ If total < 2k, merge; else ❹ transfer from the larger sibling.
template <typename KeyType>
void BTreeNodeRepairUnderFull(BTreeNode<KeyType>* node, int child) {
    // ❶ If repairing the last child, use (child-1, child) pair
    if (child == node->size) {
        child = child - 1;
    }

    // ❷
    int total = node->children[child]->size + node->children[child + 1]->size;

    if (total < 2 * node->k) {
        // ❸
        BTreeNodeMerge(node, child);
        return;
    }

    // ❹
    if (node->children[child]->size < node->children[child + 1]->size) {
        BTreeNodeTransferLeft(node, child);
    } else {
        BTreeNodeTransferRight(node, child);
    }
}

// ── BTreeNodeFindMin ─────────────────────────────────────────────────────────
// ❶ Empty node → null.
// ❷ Leaf → first key.
// ❸ Internal → recurse into leftmost child.
template <typename KeyType>
KeyType BTreeNodeFindMin(BTreeNode<KeyType>* node) {
    // ❶
    if (node->size == 0) {
        throw std::runtime_error("BTreeNodeFindMin: empty node");
    }
    // ❷
    if (node->is_leaf) {
        return node->keys[0];
    }
    // ❸
    return BTreeNodeFindMin(node->children[0]);
}

// ── BTreeNodeDelete ──────────────────────────────────────────────────────────
// ❶ Scan to find position i where key <= keys[i] (or i==size).
// ❷ Leaf + match: shift keys left, decrement size.
// ❸ Internal + match: replace with in-order successor (FindMin of right sub),
//    ❹ recursively delete that min key, then repair if needed.
// ❺ Internal + no match: recurse into children[i], repair if needed.
template <typename KeyType>
void BTreeNodeDelete(BTreeNode<KeyType>* node, KeyType key) {
    // ❶ Find i
    int i = 0;
    while (i < node->size && key > node->keys[i]) {
        i++;
    }

    // 叶节点情况
    if (node->is_leaf) {
        if (i < node->size && key == node->keys[i]) {
            // ❷ Shift left
            while (i < node->size - 1) {
                node->keys[i] = node->keys[i + 1];
                i++;
            }
            node->keys[i] = KeyType{};
            node->size     = node->size - 1;
        }
        return;
    }

    // 内部节点 + 匹配
    if (i < node->size && key == node->keys[i]) {
        // ❸ Replace with in-order successor
        KeyType min_key = BTreeNodeFindMin(node->children[i + 1]);
        node->keys[i]   = min_key;
        // ❹ Delete that successor from the right subtree
        BTreeNodeDelete(node->children[i + 1], min_key);
        if (BTreeNodeIsUnderFull(node->children[i + 1])) {
            BTreeNodeRepairUnderFull(node, i + 1);
        }
    } else {
        // ❺ Recurse into children[i]
        BTreeNodeDelete(node->children[i], key);
        if (BTreeNodeIsUnderFull(node->children[i])) {
            BTreeNodeRepairUnderFull(node, i);
        }
    }
}

// ── BTreeDelete ──────────────────────────────────────────────────────────────
template <typename KeyType>
void BTreeDelete(BTree<KeyType>* tree, KeyType key) {
    BTreeNodeDelete(tree->root, key);
    // 如果根节点变为空且不是叶节点，则提升其唯一的子节点
    if (tree->root->size == 0 && !tree->root->is_leaf) {
        BTreeNode<KeyType>* old_root = tree->root;
        tree->root = old_root->children[0];
        old_root->children[0] = nullptr;
        delete old_root;
    }
}

// ── 内存清理 ─────────────────────────────────────────────────────────────────
template <typename KeyType>
void BTreeNodeFree(BTreeNode<KeyType>* node) {
    if (!node) return;
    if (!node->is_leaf) {
        for (int i = 0; i <= node->size; i++) {
            BTreeNodeFree(node->children[i]);
        }
    }
    delete node;
}

template <typename KeyType>
void BTreeFree(BTree<KeyType>* tree) {
    BTreeNodeFree(tree->root);
    tree->root = nullptr;
}
