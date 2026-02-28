#pragma once

#include <vector>
#include <optional>

// ── Data Structures ──────────────────────────────────────────────────────────
// BTreeNode holds at most 2k keys and 2k+2 children; the extra slot (2k+1
// keys / 2k+2 children) lets a node temporarily overfill before its parent
// splits it.
template <typename KeyType = int>
struct BTreeNode {
    int                            k;
    int                            size;
    bool                           is_leaf;
    std::vector<KeyType>           keys;      // capacity 2k+1
    std::vector<BTreeNode<KeyType>*> children;  // capacity 2k+2

    BTreeNode(int k, bool is_leaf)
        : k(k), size(0), is_leaf(is_leaf),
          keys(2 * k + 1, KeyType{}),
          children(2 * k + 2, nullptr) {}
};

template <typename KeyType = int>
struct BTree {
    int k;
    BTreeNode<KeyType>* root;

    explicit BTree(int k)
        : k(k), root(new BTreeNode<KeyType>(k, true)) {}
};

// ── Search ───────────────────────────────────────────────────────────────────
template <typename KeyType>
std::optional<KeyType> BTreeNodeSearch(BTreeNode<KeyType>* node, KeyType target);

template <typename KeyType>
std::optional<KeyType> BTreeSearch(BTree<KeyType>* tree, KeyType target);

// ── Insert helpers ───────────────────────────────────────────────────────────
template <typename KeyType>
void BTreeNodeAddKey(BTreeNode<KeyType>* node, KeyType key,
                     BTreeNode<KeyType>* next_child = nullptr);

template <typename KeyType>
bool BTreeNodeIsOverFull(BTreeNode<KeyType>* node);

template <typename KeyType>
void BTreeNodeSplit(BTreeNode<KeyType>* node, int child_index);

template <typename KeyType>
void BTreeNodeInsert(BTreeNode<KeyType>* node, KeyType key);

template <typename KeyType>
void BTreeInsert(BTree<KeyType>* tree, KeyType key);

// ── Delete helpers ───────────────────────────────────────────────────────────
template <typename KeyType>
bool BTreeNodeIsUnderFull(BTreeNode<KeyType>* node);

template <typename KeyType>
void BTreeNodeMerge(BTreeNode<KeyType>* node, int index);

template <typename KeyType>
void BTreeNodeTransferLeft(BTreeNode<KeyType>* node, int index);

template <typename KeyType>
void BTreeNodeTransferRight(BTreeNode<KeyType>* node, int index);

template <typename KeyType>
void BTreeNodeRepairUnderFull(BTreeNode<KeyType>* node, int child);

template <typename KeyType>
KeyType BTreeNodeFindMin(BTreeNode<KeyType>* node);

template <typename KeyType>
void BTreeNodeDelete(BTreeNode<KeyType>* node, KeyType key);

template <typename KeyType>
void BTreeDelete(BTree<KeyType>* tree, KeyType key);

// ── Memory cleanup ───────────────────────────────────────────────────────────
template <typename KeyType>
void BTreeNodeFree(BTreeNode<KeyType>* node);

template <typename KeyType>
void BTreeFree(BTree<KeyType>* tree);

#include "BTree.tpp"
