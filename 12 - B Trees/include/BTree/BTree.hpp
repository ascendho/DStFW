#pragma once

#include <vector>
#include <optional>

// ── 数据结构 ─────────────────────────────────────────────────────────────────
// B 树节点最多保存 2k 个键和 2k+2 个子节点；额外的槽位（2k+1 个键 /
// 2k+2 个子节点）允许节点在其父节点分割之前暂时上溢。
template <typename KeyType = int>
struct BTreeNode {
    int                            k;
    int                            size;
    bool                           is_leaf;
    std::vector<KeyType>           keys;      // 容量 2k+1
    std::vector<BTreeNode<KeyType>*> children;  // 容量 2k+2

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

// ── 搜索 ─────────────────────────────────────────────────────────────────────
template <typename KeyType>
std::optional<KeyType> BTreeNodeSearch(BTreeNode<KeyType>* node, KeyType target);

template <typename KeyType>
std::optional<KeyType> BTreeSearch(BTree<KeyType>* tree, KeyType target);

// ── 插入辅助函数 ─────────────────────────────────────────────────────────────
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

// ── 删除辅助函数 ─────────────────────────────────────────────────────────────
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

// ── 内存清理 ─────────────────────────────────────────────────────────────────
template <typename KeyType>
void BTreeNodeFree(BTreeNode<KeyType>* node);

template <typename KeyType>
void BTreeFree(BTree<KeyType>* tree);

#include "BTree.tpp"
