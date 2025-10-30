#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <optional>
#include "Linear Probing/LinearProbing.hpp"

// =====================================================
// Linear Probing 哈希表测试套件
// =====================================================

class LinearProbingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试开始前的设置
    }

    void TearDown() override {
        // 每个测试结束后的清理
    }
};

// =====================================================
// 基础功能测试
// =====================================================

TEST_F(LinearProbingTest, BasicInsertAndLookup) {
    HashTable<std::string, int> ht(10);
    
    // 测试插入和查找
    EXPECT_TRUE(HashTableInsert(ht, std::string("apple"), 1));
    EXPECT_EQ(HashTableLookup(ht, std::string("apple")).value(), 1);
    EXPECT_EQ(ht.getNumKeys(), 1);
    
    // 测试插入多个元素
    EXPECT_TRUE(HashTableInsert(ht, std::string("banana"), 2));
    EXPECT_TRUE(HashTableInsert(ht, std::string("cherry"), 3));
    EXPECT_EQ(ht.getNumKeys(), 3);
    
    // 验证所有元素都能正确查找
    EXPECT_EQ(HashTableLookup(ht, std::string("apple")).value(), 1);
    EXPECT_EQ(HashTableLookup(ht, std::string("banana")).value(), 2);
    EXPECT_EQ(HashTableLookup(ht, std::string("cherry")).value(), 3);
}

TEST_F(LinearProbingTest, UpdateExistingKey) {
    HashTable<std::string, int> ht(5);
    
    // 插入初始值
    EXPECT_TRUE(HashTableInsert(ht, std::string("key1"), 100));
    EXPECT_EQ(HashTableLookup(ht, std::string("key1")).value(), 100);
    EXPECT_EQ(ht.getNumKeys(), 1);
    
    // 更新相同键的值
    EXPECT_TRUE(HashTableInsert(ht, std::string("key1"), 200));
    EXPECT_EQ(HashTableLookup(ht, std::string("key1")).value(), 200);
    EXPECT_EQ(ht.getNumKeys(), 1); // 键数量不应增加
}

TEST_F(LinearProbingTest, LookupNonExistentKey) {
    HashTable<std::string, int> ht(5);
    
    // 查找不存在的键应返回std::nullopt
    EXPECT_FALSE(HashTableLookup(ht, std::string("nonexistent")).has_value());
    
    // 插入一些元素后再测试
    HashTableInsert(ht, std::string("exists"), 42);
    EXPECT_EQ(HashTableLookup(ht, std::string("exists")).value(), 42);
    EXPECT_FALSE(HashTableLookup(ht, std::string("stillnonexistent")).has_value());
}

// =====================================================
// Linear Probing 碰撞处理测试
// =====================================================

TEST_F(LinearProbingTest, CollisionHandling) {
    // 使用较小的哈希表来强制产生碰撞
    HashTable<int, std::string> ht(3);
    
    // 插入多个可能碰撞的键
    EXPECT_TRUE(HashTableInsert(ht, 0, std::string("zero")));   // hash % 3 = 0
    EXPECT_TRUE(HashTableInsert(ht, 3, std::string("three")));  // hash % 3 = 0, 会碰撞
    EXPECT_TRUE(HashTableInsert(ht, 6, std::string("six")));    // hash % 3 = 0, 会碰撞
    
    // 验证所有值都能正确存储和检索
    EXPECT_EQ(HashTableLookup(ht, 0).value(), std::string("zero"));
    EXPECT_EQ(HashTableLookup(ht, 3).value(), std::string("three"));
    EXPECT_EQ(HashTableLookup(ht, 6).value(), std::string("six"));
    EXPECT_EQ(ht.getNumKeys(), 3);
}

TEST_F(LinearProbingTest, LinearProbingSequence) {
    // 测试线性探测的顺序
    HashTable<int, int> ht(5);
    
    // 插入会产生线性探测的元素
    EXPECT_TRUE(HashTableInsert(ht, 0, 100));  // 索引 0
    EXPECT_TRUE(HashTableInsert(ht, 5, 200));  // 索引 0 被占用，探测到索引 1
    EXPECT_TRUE(HashTableInsert(ht, 10, 300)); // 索引 0,1 被占用，探测到索引 2
    
    // 验证查找
    EXPECT_EQ(HashTableLookup(ht, 0).value(), 100);
    EXPECT_EQ(HashTableLookup(ht, 5).value(), 200);
    EXPECT_EQ(HashTableLookup(ht, 10).value(), 300);
}

// =====================================================
// 边界条件测试
// =====================================================

TEST_F(LinearProbingTest, FullTableHandling) {
    // 测试哈希表满时的行为
    HashTable<int, std::string> ht(3);
    
    // 填满哈希表
    EXPECT_TRUE(HashTableInsert(ht, 1, std::string("one")));
    EXPECT_TRUE(HashTableInsert(ht, 2, std::string("two")));
    EXPECT_TRUE(HashTableInsert(ht, 3, std::string("three")));
    EXPECT_EQ(ht.getNumKeys(), 3);
    
    // 尝试插入到已满的表中（应该失败或找到可更新的位置）
    // 根据实现，这可能成功（如果有相同key）或失败（如果表真的满了）
    bool result = HashTableInsert(ht, 4, std::string("four"));
    if (!result) {
        // 如果插入失败，验证表确实满了
        EXPECT_EQ(ht.getNumKeys(), 3);
    }
}

TEST_F(LinearProbingTest, WrapAroundProbing) {
    // 测试线性探测的环绕行为
    HashTable<int, std::string> ht(4);
    
    // 让探测序列环绕到表的开头
    EXPECT_TRUE(HashTableInsert(ht, 3, std::string("three")));  // 索引 3
    EXPECT_TRUE(HashTableInsert(ht, 7, std::string("seven")));  // 索引 3 被占用，环绕到索引 0
    
    EXPECT_EQ(HashTableLookup(ht, 3).value(), std::string("three"));
    EXPECT_EQ(HashTableLookup(ht, 7).value(), std::string("seven"));
}

// =====================================================
// 不同数据类型测试
// =====================================================

TEST_F(LinearProbingTest, IntegerKeys) {
    HashTable<int, double> ht(7);
    
    EXPECT_TRUE(HashTableInsert(ht, 42, 3.14));
    EXPECT_TRUE(HashTableInsert(ht, -5, 2.71));
    EXPECT_TRUE(HashTableInsert(ht, 0, 1.41));
    
    EXPECT_DOUBLE_EQ(HashTableLookup(ht, 42).value(), 3.14);
    EXPECT_DOUBLE_EQ(HashTableLookup(ht, -5).value(), 2.71);
    EXPECT_DOUBLE_EQ(HashTableLookup(ht, 0).value(), 1.41);
    EXPECT_FALSE(HashTableLookup(ht, 999).has_value()); // 不存在的键
}

TEST_F(LinearProbingTest, StringKeys) {
    HashTable<std::string, std::string> ht(8);
    
    EXPECT_TRUE(HashTableInsert(ht, std::string("name"), std::string("Alice")));
    EXPECT_TRUE(HashTableInsert(ht, std::string("city"), std::string("Boston")));
    EXPECT_TRUE(HashTableInsert(ht, std::string("country"), std::string("USA")));
    
    EXPECT_EQ(HashTableLookup(ht, std::string("name")).value(), std::string("Alice"));
    EXPECT_EQ(HashTableLookup(ht, std::string("city")).value(), std::string("Boston"));
    EXPECT_EQ(HashTableLookup(ht, std::string("country")).value(), std::string("USA"));
    EXPECT_FALSE(HashTableLookup(ht, std::string("unknown")).has_value()); // 不存在的键
}

// =====================================================
// 性能和压力测试
// =====================================================

TEST_F(LinearProbingTest, StressTest) {
    HashTable<int, int> ht(100);
    
    // 插入大量元素
    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(HashTableInsert(ht, i, i * 2));
    }
    EXPECT_EQ(ht.getNumKeys(), 50);
    
    // 验证所有元素
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(HashTableLookup(ht, i).value(), i * 2);
    }
    
    // 更新一些元素
    for (int i = 0; i < 25; ++i) {
        EXPECT_TRUE(HashTableInsert(ht, i, i * 3));
    }
    EXPECT_EQ(ht.getNumKeys(), 50); // 数量不应改变
    
    // 验证更新的元素
    for (int i = 0; i < 25; ++i) {
        EXPECT_EQ(HashTableLookup(ht, i).value(), i * 3);
    }
    for (int i = 25; i < 50; ++i) {
        EXPECT_EQ(HashTableLookup(ht, i).value(), i * 2);
    }
}

// =====================================================
// HashFunction 函数测试
// =====================================================

TEST_F(LinearProbingTest, HashFunctionConsistency) {
    // 测试哈希函数的一致性
    size_t table_size = 100;
    
    // 相同输入应产生相同输出
    EXPECT_EQ(HashFunction(42, table_size), HashFunction(42, table_size));
    EXPECT_EQ(HashFunction(std::string("test"), table_size), 
              HashFunction(std::string("test"), table_size));
    
    // 输出应在有效范围内
    for (int i = 0; i < 20; ++i) {
        size_t hash = HashFunction(i, table_size);
        EXPECT_LT(hash, table_size);
        EXPECT_GE(hash, 0);
    }
}

TEST_F(LinearProbingTest, HashFunctionDistribution) {
    // 简单的分布测试
    size_t table_size = 10;
    std::vector<int> bucket_counts(table_size, 0);
    
    // 计算一些字符串的哈希分布
    std::vector<std::string> test_strings = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "honeydew", "kiwi", "lemon"
    };
    
    for (const auto& str : test_strings) {
        size_t hash = HashFunction(str, table_size);
        bucket_counts[hash]++;
    }
    
    // 验证没有桶是完全空的（这是一个宽松的分布测试）
    int empty_buckets = 0;
    for (int count : bucket_counts) {
        if (count == 0) empty_buckets++;
    }
    
    // 对于10个元素和10个桶，期望大部分桶都有元素
    EXPECT_LE(empty_buckets, 5); // 允许一些桶为空
}

// =====================================================
// 内存管理测试
// =====================================================

TEST_F(LinearProbingTest, MemoryManagement) {
    // 创建一个作用域来测试析构函数
    {
        HashTable<std::string, int> ht(5);
        
        // 插入一些元素
        HashTableInsert(ht, std::string("key1"), 1);
        HashTableInsert(ht, std::string("key2"), 2);  
        HashTableInsert(ht, std::string("key3"), 3);
        
        // 验证元素存在
        EXPECT_EQ(HashTableLookup(ht, std::string("key1")).value(), 1);
        EXPECT_EQ(HashTableLookup(ht, std::string("key2")).value(), 2);
        EXPECT_EQ(HashTableLookup(ht, std::string("key3")).value(), 3);
        
        // ht在这里会被自动析构，应该正确清理内存
    }
    
    // 如果到这里没有内存错误，说明析构函数工作正常
    SUCCEED();
}