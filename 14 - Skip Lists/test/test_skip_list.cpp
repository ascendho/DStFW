#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "SkipList/SkipList.hpp"

// ── 辅助函数 ──────────────────────────────────────────────────────────────────

// 遍历第 0 层收集所有键
template <typename K, typename V>
static std::vector<K> AllKeys(SkipList<K, V>& list) {
    std::vector<K> keys;
    auto* cur = list.front->next[0];
    while (cur) {
        keys.push_back(cur->key);
        cur = cur->next[0];
    }
    return keys;
}

// 验证跳表的结构不变量：
//   - 第 0 层的键严格有序
//   - 每个更高层级的链是其下一层链的子序列
//   - top_level 正确（具有至少一个节点的最高层级，空表时为 0）
static void Validate(SkipList<int, int>& list) {
    // 检查第 0 层是否有序
    auto* cur = list.front->next[0];
    while (cur && cur->next[0]) {
        EXPECT_LT(cur->key, cur->next[0]->key)
            << "Level-0 keys must be strictly increasing";
        cur = cur->next[0];
    }

    // 每个更高层级的链必须是其下一层的子集
    for (int lv = 1; lv <= list.top_level; lv++) {
        auto* hi = list.front->next[lv];
        auto* lo = list.front->next[lv - 1];
        while (hi) {
            // 向前推进 lo 直到找到 hi 的键
            while (lo && lo->key < hi->key) lo = lo->next[lv - 1];
            ASSERT_NE(lo, nullptr) << "Higher-level node not found in lower level";
            EXPECT_EQ(lo, hi)
                << "Higher-level node must be the same pointer as lower-level node";
            hi = hi->next[lv];
        }
    }

    // top_level 有效性
    if (list.front->next[0] == nullptr) {
        // 空表 ── top_level 应为 0
        EXPECT_EQ(list.top_level, 0);
    } else {
        // front->next[top_level] 应非空
        EXPECT_NE(list.front->next[list.top_level], nullptr)
            << "top_level should point to a level with at least one node";
        // top_level 以上的层级应全部为空
        for (int lv = list.top_level + 1; lv <= list.max_level; lv++) {
            EXPECT_EQ(list.front->next[lv], nullptr)
                << "level " << lv << " above top_level should be empty";
        }
    }
}

// 固定种子的跳表，用于确定性测试
static SkipList<int, int> MakeList(int max_level = 16,
                                   unsigned seed = 42) {
    return SkipList<int, int>(max_level, 0.5, seed);
}

// ── 搜索测试 ──────────────────────────────────────────────────────────────────

TEST(SkipListSearch, EmptyList) {
    auto list = MakeList();
    EXPECT_FALSE(SkipListSearch(list, 5).has_value());
}

TEST(SkipListSearch, SingleElement) {
    auto list = MakeList();
    SkipListInsert(list, 10, 100);
    auto val = SkipListSearch(list, 10);
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 100);
    EXPECT_FALSE(SkipListSearch(list, 5).has_value());
}

TEST(SkipListSearch, MultipleElements) {
    auto list = MakeList();
    for (int i = 1; i <= 10; i++) SkipListInsert(list, i, i * 10);
    for (int i = 1; i <= 10; i++) {
        auto val = SkipListSearch(list, i);
        ASSERT_TRUE(val.has_value()) << "Missing key " << i;
        EXPECT_EQ(val.value(), i * 10);
    }
    EXPECT_FALSE(SkipListSearch(list, 0).has_value());
    EXPECT_FALSE(SkipListSearch(list, 11).has_value());
}

// ── 插入测试 ──────────────────────────────────────────────────────────────────

TEST(SkipListInsert, InsertMaintainsSortedOrder) {
    auto list = MakeList();
    for (int v : {5, 3, 8, 1, 4, 7, 9, 2, 6}) SkipListInsert(list, v, v);
    auto keys = AllKeys(list);
    EXPECT_EQ(keys, (std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));
    Validate(list);
}

TEST(SkipListInsert, InsertDuplicateUpdatesValue) {
    auto list = MakeList();
    SkipListInsert(list, 5, 50);
    SkipListInsert(list, 5, 99);
    auto val = SkipListSearch(list, 5);
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 99);
    auto keys = AllKeys(list);
    EXPECT_EQ(keys.size(), 1u); // 无重复节点
}

TEST(SkipListInsert, InsertReverseOrder) {
    auto list = MakeList();
    for (int i = 20; i >= 1; i--) SkipListInsert(list, i, i);
    auto keys = AllKeys(list);
    std::vector<int> expected;
    for (int i = 1; i <= 20; i++) expected.push_back(i);
    EXPECT_EQ(keys, expected);
    Validate(list);
}

TEST(SkipListInsert, InsertLargeSequential) {
    auto list = MakeList();
    for (int i = 1; i <= 100; i++) SkipListInsert(list, i, i);
    Validate(list);
    auto keys = AllKeys(list);
    EXPECT_EQ((int)keys.size(), 100);
    for (int i = 0; i < 100; i++) EXPECT_EQ(keys[i], i + 1);
}

TEST(SkipListInsert, InsertRandomOrder) {
    auto list = MakeList(16, 123);
    std::vector<int> values;
    for (int i = 1; i <= 50; i++) values.push_back(i);
    std::mt19937 rng(77);
    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) SkipListInsert(list, v, v * 10);
    Validate(list);
    auto keys = AllKeys(list);
    std::sort(values.begin(), values.end());
    EXPECT_EQ(keys, values);
}

// ── 删除测试 ──────────────────────────────────────────────────────────────────

TEST(SkipListDelete, DeleteSingleElement) {
    auto list = MakeList();
    SkipListInsert(list, 10, 100);
    SkipListDelete(list, 10);
    EXPECT_FALSE(SkipListSearch(list, 10).has_value());
    EXPECT_TRUE(AllKeys(list).empty());
}

TEST(SkipListDelete, DeleteNonExistent) {
    auto list = MakeList();
    for (int v : {1, 3, 5}) SkipListInsert(list, v, v);
    SkipListDelete(list, 99); // 无操作
    auto keys = AllKeys(list);
    EXPECT_EQ(keys, (std::vector<int>{1, 3, 5}));
}

TEST(SkipListDelete, DeleteFromMiddle) {
    auto list = MakeList();
    for (int v : {1, 2, 3, 4, 5}) SkipListInsert(list, v, v);
    SkipListDelete(list, 3);
    EXPECT_FALSE(SkipListSearch(list, 3).has_value());
    auto keys = AllKeys(list);
    EXPECT_EQ(keys, (std::vector<int>{1, 2, 4, 5}));
    Validate(list);
}

TEST(SkipListDelete, DeleteFirst) {
    auto list = MakeList();
    for (int v : {1, 2, 3, 4, 5}) SkipListInsert(list, v, v);
    SkipListDelete(list, 1);
    auto keys = AllKeys(list);
    EXPECT_EQ(keys, (std::vector<int>{2, 3, 4, 5}));
    Validate(list);
}

TEST(SkipListDelete, DeleteLast) {
    auto list = MakeList();
    for (int v : {1, 2, 3, 4, 5}) SkipListInsert(list, v, v);
    SkipListDelete(list, 5);
    auto keys = AllKeys(list);
    EXPECT_EQ(keys, (std::vector<int>{1, 2, 3, 4}));
    Validate(list);
}

TEST(SkipListDelete, DeleteAllKeys) {
    auto list = MakeList();
    std::vector<int> vals = {5, 3, 8, 1, 4, 7, 9, 2, 6};
    for (int v : vals) SkipListInsert(list, v, v);
    for (int v : vals) {
        SkipListDelete(list, v);
        EXPECT_FALSE(SkipListSearch(list, v).has_value());
    }
    EXPECT_TRUE(AllKeys(list).empty());
}

TEST(SkipListDelete, DeleteLargeRandomSet) {
    auto list = MakeList(16, 99);
    std::vector<int> values;
    for (int i = 1; i <= 100; i++) values.push_back(i);
    for (int v : values) SkipListInsert(list, v, v);
    std::mt19937 rng(200);
    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) {
        SkipListDelete(list, v);
        EXPECT_FALSE(SkipListSearch(list, v).has_value());
    }
    EXPECT_TRUE(AllKeys(list).empty());
}

// ── 交替插入/删除 ────────────────────────────────────────────────────────────

TEST(SkipListMixed, InterleavedInsertDelete) {
    auto list = MakeList();
    for (int i = 1; i <= 30; i++) SkipListInsert(list, i, i);
    // 删除奇数
    for (int i = 1; i <= 30; i += 2) SkipListDelete(list, i);
    Validate(list);
    auto keys = AllKeys(list);
    std::vector<int> evens;
    for (int i = 2; i <= 30; i += 2) evens.push_back(i);
    EXPECT_EQ(keys, evens);
    // 重新插入奇数
    for (int i = 1; i <= 30; i += 2) SkipListInsert(list, i, i);
    Validate(list);
    keys = AllKeys(list);
    std::vector<int> all;
    for (int i = 1; i <= 30; i++) all.push_back(i);
    EXPECT_EQ(keys, all);
}

// ── 字符串键 ──────────────────────────────────────────────────────────────────

TEST(SkipListString, StringKeysWork) {
    SkipList<std::string, int> list(8, 0.5, 42);
    SkipListInsert(list, std::string("charlie"), 3);
    SkipListInsert(list, std::string("alice"),   1);
    SkipListInsert(list, std::string("bob"),     2);
    auto val = SkipListSearch(list, std::string("bob"));
    ASSERT_TRUE(val.has_value());
    EXPECT_EQ(val.value(), 2);
    EXPECT_FALSE(SkipListSearch(list, std::string("dave")).has_value());
}

// ── 随机层级分布 ──────────────────────────────────────────────────────────────
// 当 p=0.5 时，约 50% 的节点应在第 0 层，约 25% 在第 1 层，依此类推。
TEST(SkipListRandomLevel, DistributionRoughlyGeometric) {
    SkipList<int, int> list(16, 0.5, 777);
    const int N = 10000;
    std::vector<int> counts(17, 0);
    for (int i = 0; i < N; i++) {
        int lv = SkipListRandomLevel(list);
        ASSERT_GE(lv, 0);
        ASSERT_LE(lv, list.max_level);
        counts[lv]++;
    }
    // 第 0 层应约占 50%
    double frac0 = static_cast<double>(counts[0]) / N;
    EXPECT_GT(frac0, 0.40);
    EXPECT_LT(frac0, 0.60);
    // 第 1 层应约占 25%
    double frac1 = static_cast<double>(counts[1]) / N;
    EXPECT_GT(frac1, 0.18);
    EXPECT_LT(frac1, 0.32);
}

// ── 删除时 top_level 调整 ────────────────────────────────────────────────────
TEST(SkipListTopLevel, TopLevelDecreasesOnDelete) {
    auto list = MakeList(16, 500);
    for (int i = 1; i <= 50; i++) SkipListInsert(list, i, i);
    int initial_top = list.top_level;
    // 删除全部
    for (int i = 1; i <= 50; i++) SkipListDelete(list, i);
    EXPECT_EQ(list.top_level, 0);
    EXPECT_LE(list.top_level, initial_top);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
