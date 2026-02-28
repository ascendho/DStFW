#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <random>
#include "BloomFilter/BloomFilter.hpp"

// ── 构造 ──────────────────────────────────────────────────────────────────────

TEST(BloomFilterConstruct, DefaultBinsAreZero) {
    BloomFilter<int> bf(100, 3);
    EXPECT_EQ(bf.size, 100);
    EXPECT_EQ(bf.k, 3);
    EXPECT_EQ((int)bf.bins.size(), 100);
    for (int i = 0; i < 100; i++) {
        EXPECT_FALSE(bf.bins[i]) << "bin " << i << " should start as 0";
    }
}

TEST(BloomFilterConstruct, HashFunctionsCreated) {
    BloomFilter<int> bf(200, 5);
    EXPECT_EQ((int)bf.h.size(), 5);
}

// ── 空过滤器上的查找 ──────────────────────────────────────────────────────────

TEST(BloomFilterLookup, EmptyFilterReturnsFalse) {
    BloomFilter<int> bf(256, 3);
    for (int v : {1, 10, 100, -5, 999}) {
        EXPECT_FALSE(BloomFilterLookup(bf, v)) << "Empty filter should not contain " << v;
    }
}

TEST(BloomFilterLookup, EmptyStringFilter) {
    BloomFilter<std::string> bf(512, 4);
    EXPECT_FALSE(BloomFilterLookup(bf, std::string("hello")));
    EXPECT_FALSE(BloomFilterLookup(bf, std::string("world")));
}

// ── 无假阴性 ──────────────────────────────────────────────────────────────────

TEST(BloomFilterInsert, InsertedKeyAlwaysFound) {
    BloomFilter<int> bf(1000, 3);
    std::vector<int> inserted = {1, 2, 3, 4, 5, 100, 200, 999};
    for (int v : inserted) BloomFilterInsertKey(bf, v);
    for (int v : inserted) {
        EXPECT_TRUE(BloomFilterLookup(bf, v))
            << "Inserted key " << v << " must always be found (no false negatives)";
    }
}

TEST(BloomFilterInsert, InsertedStringsAlwaysFound) {
    BloomFilter<std::string> bf(2000, 4);
    std::vector<std::string> words = {"bloom", "filter", "data", "structure",
                                       "hash", "function", "false", "positive"};
    for (const auto& w : words) BloomFilterInsertKey(bf, w);
    for (const auto& w : words) {
        EXPECT_TRUE(BloomFilterLookup(bf, w))
            << "Inserted string \"" << w << "\" must always be found";
    }
}

TEST(BloomFilterInsert, NoFalseNegativesLargeSet) {
    BloomFilter<int> bf(5000, 5);
    std::vector<int> inserted;
    for (int i = 0; i < 200; i++) inserted.push_back(i);
    for (int v : inserted) BloomFilterInsertKey(bf, v);
    for (int v : inserted) {
        EXPECT_TRUE(BloomFilterLookup(bf, v))
            << "No false negatives: " << v << " must be found";
    }
}

// ── 单次插入 ──────────────────────────────────────────────────────────────────

TEST(BloomFilterInsert, SingleInsert) {
    BloomFilter<int> bf(256, 3);
    BloomFilterInsertKey(bf, 42);
    EXPECT_TRUE(BloomFilterLookup(bf, 42));
    // 统计设置了多少个槽位 ── 最多应为 k=3
    int count = 0;
    for (int i = 0; i < bf.size; i++) if (bf.bins[i]) count++;
    EXPECT_LE(count, bf.k);
}

// ── 槽位正确设置 ──────────────────────────────────────────────────────────────

TEST(BloomFilterInsert, ExactlyKBinsSetPerInsert) {
    // 过滤器足够大时，插入一个键最多应设置 k 个不同的槽位
    //（如果哈希函数之间发生碰撞，则可能更少）
    BloomFilter<std::string> bf(10000, 3);
    BloomFilterInsertKey(bf, std::string("test_key"));
    int count = 0;
    for (int i = 0; i < bf.size; i++) if (bf.bins[i]) count++;
    EXPECT_GE(count, 1);
    EXPECT_LE(count, bf.k);
}

// ── 假阳性率 ──────────────────────────────────────────────────────────────────

// 插入 n 个元素后，从不相交的探测集统计假阳性数量。
// 在宽裕的参数下（m=10*n, k=3），假阳性率应远低于 5%。
TEST(BloomFilterFPR, LowFalsePositiveRateWithGenerousParams) {
    const int n = 100;
    BloomFilter<int> bf(10 * n, 3);
    // 插入键 0..n-1
    for (int i = 0; i < n; i++) BloomFilterInsertKey(bf, i);
    // 探测键 n..2n-1（不相交）
    int false_positives = 0;
    for (int i = n; i < 2 * n; i++) {
        if (BloomFilterLookup(bf, i)) false_positives++;
    }
    double fpr = static_cast<double>(false_positives) / n;
    EXPECT_LT(fpr, 0.05) << "FPR=" << fpr << " should be under 5% with m=10n, k=3";
}

// 理论假阳性率公式：m=1000, n=100, k=3 → ~1.7%（见书中表 13-1）
TEST(BloomFilterFPR, TheoreticalFPRIsReasonable) {
    double fpr = BloomFilterFalsePositiveRate(100, 1000, 3);
    EXPECT_GT(fpr, 0.0);
    EXPECT_LT(fpr, 0.05);   // 书中表 13-1：m=1000, k=3, n=100 时约为 0.017
}

TEST(BloomFilterFPR, LargerFilterHasLowerFPR) {
    double fpr_small = BloomFilterFalsePositiveRate(100, 400, 3);
    double fpr_large = BloomFilterFalsePositiveRate(100, 1000, 3);
    EXPECT_LT(fpr_large, fpr_small) << "Larger filter should have lower FPR";
}

TEST(BloomFilterFPR, MoreItemsIncreaseFPR) {
    double fpr_few  = BloomFilterFalsePositiveRate(50,  1000, 3);
    double fpr_many = BloomFilterFalsePositiveRate(200, 1000, 3);
    EXPECT_LT(fpr_few, fpr_many) << "More inserted items → higher FPR";
}

// ── 不同参数组合 ──────────────────────────────────────────────────────────────

TEST(BloomFilterParams, K1WorksCorrectly) {
    BloomFilter<int> bf(500, 1);
    for (int v : {10, 20, 30}) BloomFilterInsertKey(bf, v);
    for (int v : {10, 20, 30}) EXPECT_TRUE(BloomFilterLookup(bf, v));
}

TEST(BloomFilterParams, K7WorksCorrectly) {
    BloomFilter<int> bf(2000, 7);
    for (int i = 0; i < 50; i++) BloomFilterInsertKey(bf, i);
    for (int i = 0; i < 50; i++) EXPECT_TRUE(BloomFilterLookup(bf, i));
}

// ── 自定义哈希函数 ────────────────────────────────────────────────────────────

TEST(BloomFilterCustomHash, CustomHashFunctions) {
    // 两个简单的对整数键取模的哈希函数
    int m = 100;
    std::vector<std::function<size_t(const int&)>> fns = {
        [m](const int& k) -> size_t { return static_cast<size_t>((k * 2654435761ULL) % m); },
        [m](const int& k) -> size_t { return static_cast<size_t>((k * 2246822519ULL) % m); },
    };
    BloomFilter<int> bf(m, 2, fns);
    BloomFilterInsertKey(bf, 42);
    BloomFilterInsertKey(bf, 7);
    EXPECT_TRUE(BloomFilterLookup(bf, 42));
    EXPECT_TRUE(BloomFilterLookup(bf, 7));
}

// ── 幂等插入 ──────────────────────────────────────────────────────────────────

TEST(BloomFilterInsert, InsertSameKeyMultipleTimes) {
    BloomFilter<std::string> bf(256, 3);
    for (int i = 0; i < 10; i++) BloomFilterInsertKey(bf, std::string("duplicate"));
    EXPECT_TRUE(BloomFilterLookup(bf, std::string("duplicate")));
    // 第一次插入后槽位计数不应增长
    int count_after_1 = 0;
    BloomFilter<std::string> bf2(256, 3);
    BloomFilterInsertKey(bf2, std::string("duplicate"));
    for (int i = 0; i < bf2.size; i++) if (bf2.bins[i]) count_after_1++;
    int count_after_n = 0;
    for (int i = 0; i < bf.size; i++) if (bf.bins[i]) count_after_n++;
    EXPECT_EQ(count_after_1, count_after_n);
}

// ── 查找提前退出 ──────────────────────────────────────────────────────────────

// （行为测试：仅验证正确结果 ── 提前退出是内部优化）
TEST(BloomFilterLookup, EarlyExitOnZeroBin) {
    BloomFilter<int> bf(1000, 5);
    // 不插入任何元素 ── 遇到第一个为 0 的槽位应返回 false
    EXPECT_FALSE(BloomFilterLookup(bf, 12345));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
