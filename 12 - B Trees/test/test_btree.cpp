#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include "BTree/BTree.hpp"

// ── 辅助函数 ─────────────────────────────────────────────────────────────────

// 通过中序遍历按排序顺序收集所有键
static void InOrder(BTreeNode<int>* node, std::vector<int>& out) {
    if (!node) return;
    for (int i = 0; i < node->size; i++) {
        if (!node->is_leaf) InOrder(node->children[i], out);
        out.push_back(node->keys[i]);
    }
    if (!node->is_leaf) InOrder(node->children[node->size], out);
}

static std::vector<int> AllKeys(BTree<int>* tree) {
    std::vector<int> keys;
    InOrder(tree->root, keys);
    return keys;
}

// 验证 B 树不变量：
//   - 叶节点深度一致
//   - 非根节点满足 k <= size <= 2k
//   - 根节点满足 0 <= size <= 2k
//   - 每个节点内的键有序
//   - 返回叶节点深度（所有叶节点深度必须相等）
static int ValidateNode(BTreeNode<int>* node, bool is_root, int depth) {
    // 节点内键的顺序
    for (int i = 0; i + 1 < node->size; i++) {
        EXPECT_LT(node->keys[i], node->keys[i + 1])
            << "Node keys out of order at depth " << depth;
    }
    // 大小限制
    if (!is_root) {
        EXPECT_GE(node->size, node->k) << "Node underfull at depth " << depth;
    }
    EXPECT_LE(node->size, 2 * node->k) << "Node overfull at depth " << depth;

    if (node->is_leaf) return depth;

    int leaf_depth = -1;
    for (int i = 0; i <= node->size; i++) {
        EXPECT_NE(node->children[i], nullptr) << "Null child at depth " << depth;
        if (node->children[i]) {
            int d = ValidateNode(node->children[i], false, depth + 1);
            if (leaf_depth == -1) leaf_depth = d;
            else EXPECT_EQ(leaf_depth, d) << "Unequal leaf depths";
        }
    }
    return leaf_depth;
}

static void Validate(BTree<int>* tree) {
    ValidateNode(tree->root, true, 0);
}

// ── 搜索测试 ─────────────────────────────────────────────────────────────────

TEST(BTreeSearch, EmptyTree) {
    BTree<int> tree(2);
    EXPECT_FALSE(BTreeSearch(&tree, 5).has_value());
    BTreeFree(&tree);
}

TEST(BTreeSearch, SingleElement) {
    BTree<int> tree(2);
    BTreeInsert(&tree, 10);
    EXPECT_TRUE(BTreeSearch(&tree, 10).has_value());
    EXPECT_EQ(BTreeSearch(&tree, 10).value(), 10);
    EXPECT_FALSE(BTreeSearch(&tree, 5).has_value());
    BTreeFree(&tree);
}

TEST(BTreeSearch, MultipleElements) {
    BTree<int> tree(2);
    for (int v : {5, 3, 7, 1, 4, 6, 8}) BTreeInsert(&tree, v);
    for (int v : {1, 3, 4, 5, 6, 7, 8}) {
        auto found = BTreeSearch(&tree, v);
        EXPECT_TRUE(found.has_value()) << "Missing " << v;
        EXPECT_EQ(found.value(), v);
    }
    EXPECT_FALSE(BTreeSearch(&tree, 9).has_value());
    EXPECT_FALSE(BTreeSearch(&tree, 0).has_value());
    BTreeFree(&tree);
}

// ── 插入测试 ─────────────────────────────────────────────────────────────────

TEST(BTreeInsert, InsertNoSplit) {
    // k=2：叶节点最多可容纳 4 个键而无需分割
    BTree<int> tree(2);
    BTreeInsert(&tree, 10);
    BTreeInsert(&tree, 20);
    BTreeInsert(&tree, 30);
    BTreeInsert(&tree, 40);
    EXPECT_EQ(tree.root->size, 4);
    EXPECT_TRUE(tree.root->is_leaf);
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertLeafSplit) {
    // k=2：插入第 5 个键会触发叶节点分割
    BTree<int> tree(2);
    for (int v : {10, 20, 30, 40, 50}) BTreeInsert(&tree, v);
    EXPECT_FALSE(tree.root->is_leaf);
    EXPECT_EQ(tree.root->size, 1);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    EXPECT_EQ(keys, (std::vector<int>{10, 20, 30, 40, 50}));
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertRootSplit) {
    // 通过插入足够多的键迫使根节点本身分割
    BTree<int> tree(2);
    for (int i = 1; i <= 15; i++) BTreeInsert(&tree, i);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    std::vector<int> expected;
    for (int i = 1; i <= 15; i++) expected.push_back(i);
    EXPECT_EQ(keys, expected);
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertDuplicate) {
    BTree<int> tree(2);
    BTreeInsert(&tree, 5);
    BTreeInsert(&tree, 5);
    auto keys = AllKeys(&tree);
    EXPECT_EQ(keys.size(), 1u);
    EXPECT_EQ(keys[0], 5);
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertReverseOrder) {
    BTree<int> tree(2);
    for (int i = 20; i >= 1; i--) BTreeInsert(&tree, i);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    std::vector<int> expected;
    for (int i = 1; i <= 20; i++) expected.push_back(i);
    EXPECT_EQ(keys, expected);
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertLargeSequential) {
    BTree<int> tree(3);
    for (int i = 1; i <= 100; i++) BTreeInsert(&tree, i);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    EXPECT_EQ((int)keys.size(), 100);
    for (int i = 0; i < 100; i++) EXPECT_EQ(keys[i], i + 1);
    BTreeFree(&tree);
}

TEST(BTreeInsert, InsertRandomOrder) {
    BTree<int> tree(2);
    std::vector<int> values;
    for (int i = 1; i <= 50; i++) values.push_back(i);
    std::mt19937 rng(42);
    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) BTreeInsert(&tree, v);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    std::sort(values.begin(), values.end());
    EXPECT_EQ(keys, values);
    BTreeFree(&tree);
}

TEST(BTreeInsert, TreeWithK1) {
    // k=1：节点保存 1–2 个键（2-3 树）
    BTree<int> tree(1);
    for (int i = 1; i <= 20; i++) BTreeInsert(&tree, i);
    Validate(&tree);
    auto keys = AllKeys(&tree);
    EXPECT_EQ((int)keys.size(), 20);
    BTreeFree(&tree);
}

// ── 删除测试 ─────────────────────────────────────────────────────────────────

TEST(BTreeDelete, DeleteFromLeafNoRepair) {
    BTree<int> tree(2);
    for (int v : {1, 2, 3, 4, 5, 6, 7}) BTreeInsert(&tree, v);
    BTreeDelete(&tree, 3);
    EXPECT_FALSE(BTreeSearch(&tree, 3).has_value());
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteFromInternalNode) {
    BTree<int> tree(2);
    for (int v : {10, 20, 30, 40, 50, 60, 70}) BTreeInsert(&tree, v);
    // 强制内部节点键删除
    BTreeDelete(&tree, 30);
    EXPECT_FALSE(BTreeSearch(&tree, 30).has_value());
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteRequiresMerge) {
    BTree<int> tree(2);
    for (int i = 1; i <= 10; i++) BTreeInsert(&tree, i);
    // 删除键直到触发合并
    for (int v : {1, 2, 3}) {
        BTreeDelete(&tree, v);
        EXPECT_FALSE(BTreeSearch(&tree, v).has_value());
    }
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteRequiresTransfer) {
    BTree<int> tree(2);
    // 构建一棵树，使得某个兄弟节点有足够多的键来转移
    for (int i = 1; i <= 20; i++) BTreeInsert(&tree, i);
    BTreeDelete(&tree, 1);
    BTreeDelete(&tree, 2);
    EXPECT_FALSE(BTreeSearch(&tree, 1).has_value());
    EXPECT_FALSE(BTreeSearch(&tree, 2).has_value());
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteNonExistentKey) {
    BTree<int> tree(2);
    for (int v : {1, 2, 3, 4, 5}) BTreeInsert(&tree, v);
    BTreeDelete(&tree, 99); // 无操作
    Validate(&tree);
    auto keys = AllKeys(&tree);
    EXPECT_EQ(keys, (std::vector<int>{1, 2, 3, 4, 5}));
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteRootCollapses) {
    // k=2；插入刚好足够的键使根节点恰好有两个子节点，
    // 然后删除除一个键以外的所有键以迫使根节点坍缩
    BTree<int> tree(2);
    for (int i = 1; i <= 5; i++) BTreeInsert(&tree, i);
    for (int i = 1; i <= 4; i++) BTreeDelete(&tree, i);
    // 根节点现在应坍缩为单个叶节点
    EXPECT_TRUE(tree.root->is_leaf);
    Validate(&tree);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteAllKeys) {
    BTree<int> tree(2);
    std::vector<int> vals = {5, 3, 8, 1, 4, 7, 9, 2, 6};
    for (int v : vals) BTreeInsert(&tree, v);
    for (int v : vals) {
        BTreeDelete(&tree, v);
        EXPECT_FALSE(BTreeSearch(&tree, v).has_value());
    }
    EXPECT_EQ(tree.root->size, 0);
    BTreeFree(&tree);
}

TEST(BTreeDelete, DeleteLargeRandomSet) {
    BTree<int> tree(3);
    std::vector<int> values;
    for (int i = 1; i <= 100; i++) values.push_back(i);
    std::mt19937 rng(123);
    for (int v : values) BTreeInsert(&tree, v);
    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) {
        BTreeDelete(&tree, v);
        EXPECT_FALSE(BTreeSearch(&tree, v).has_value());
    }
    EXPECT_EQ(tree.root->size, 0);
    BTreeFree(&tree);
}

TEST(BTreeDelete, InterleavedInsertDelete) {
    BTree<int> tree(2);
    for (int i = 1; i <= 30; i++) BTreeInsert(&tree, i);
    for (int i = 1; i <= 30; i += 2) BTreeDelete(&tree, i); // 删除奇数
    Validate(&tree);
    auto keys = AllKeys(&tree);
    std::vector<int> evens;
    for (int i = 2; i <= 30; i += 2) evens.push_back(i);
    EXPECT_EQ(keys, evens);
    // 重新插入已删除的奇数
    for (int i = 1; i <= 30; i += 2) BTreeInsert(&tree, i);
    Validate(&tree);
    keys = AllKeys(&tree);
    std::vector<int> all;
    for (int i = 1; i <= 30; i++) all.push_back(i);
    EXPECT_EQ(keys, all);
    BTreeFree(&tree);
}

// ── 查找最小值测试 ───────────────────────────────────────────────────────────

TEST(BTreeFindMin, FindMinAfterInserts) {
    BTree<int> tree(2);
    for (int v : {5, 3, 8, 1, 7}) BTreeInsert(&tree, v);
    EXPECT_EQ(BTreeNodeFindMin(tree.root), 1);
    BTreeFree(&tree);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
