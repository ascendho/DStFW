#include <gtest/gtest.h>
#include "Trie.hpp"

// 测试基本插入和搜索
TEST(TrieTest, InsertAndSearch) {
    Trie trie;
    
    trie.TrieInsert("cat");
    trie.TrieInsert("car");
    trie.TrieInsert("card");
    trie.TrieInsert("care");
    trie.TrieInsert("careful");
    trie.TrieInsert("can");
    
    TrieNode* node = trie.TrieSearch("cat");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("car");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("card");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("careful");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试搜索不存在的字符串
TEST(TrieTest, SearchNonExistent) {
    Trie trie;
    
    trie.TrieInsert("cat");
    trie.TrieInsert("car");
    
    TrieNode* node = trie.TrieSearch("dog");
    EXPECT_EQ(node, nullptr);
    
    node = trie.TrieSearch("ca");
    EXPECT_EQ(node, nullptr);
    
    node = trie.TrieSearch("cats");
    EXPECT_EQ(node, nullptr);
}

// 测试空 Trie
TEST(TrieTest, EmptyTrie) {
    Trie trie;
    
    TrieNode* node = trie.TrieSearch("cat");
    EXPECT_EQ(node, nullptr);
}

// 测试前缀
TEST(TrieTest, Prefix) {
    Trie trie;
    
    trie.TrieInsert("care");
    trie.TrieInsert("careful");
    
    // "care" 应该存在
    TrieNode* node = trie.TrieSearch("care");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    // "careful" 应该存在
    node = trie.TrieSearch("careful");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    // "car" 不应该存在（虽然是前缀）
    node = trie.TrieSearch("car");
    EXPECT_EQ(node, nullptr);
}

// 测试删除叶子节点
TEST(TrieTest, DeleteLeaf) {
    Trie trie;
    
    trie.TrieInsert("cat");
    trie.TrieInsert("car");
    trie.TrieInsert("card");
    
    // 删除 "card"
    trie.TrieDelete("card");
    
    TrieNode* node = trie.TrieSearch("card");
    EXPECT_EQ(node, nullptr);
    
    // 确认其他字符串仍然存在
    node = trie.TrieSearch("cat");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("car");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试删除中间节点
TEST(TrieTest, DeleteMiddleNode) {
    Trie trie;
    
    trie.TrieInsert("car");
    trie.TrieInsert("card");
    
    // 删除 "car"
    trie.TrieDelete("car");
    
    TrieNode* node = trie.TrieSearch("car");
    EXPECT_EQ(node, nullptr);
    
    // 确认 "card" 仍然存在
    node = trie.TrieSearch("card");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试删除不存在的字符串
TEST(TrieTest, DeleteNonExistent) {
    Trie trie;
    
    trie.TrieInsert("cat");
    
    // 删除不存在的字符串不应导致错误
    trie.TrieDelete("dog");
    
    // 确认 "cat" 仍然存在
    TrieNode* node = trie.TrieSearch("cat");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试重复插入
TEST(TrieTest, DuplicateInsert) {
    Trie trie;
    
    trie.TrieInsert("cat");
    trie.TrieInsert("cat");
    
    TrieNode* node = trie.TrieSearch("cat");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试单个字符
TEST(TrieTest, SingleCharacter) {
    Trie trie;
    
    trie.TrieInsert("a");
    trie.TrieInsert("b");
    
    TrieNode* node = trie.TrieSearch("a");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("b");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
    
    node = trie.TrieSearch("c");
    EXPECT_EQ(node, nullptr);
}

// 测试空字符串
TEST(TrieTest, EmptyString) {
    Trie trie;
    
    trie.TrieInsert("");
    
    TrieNode* node = trie.TrieSearch("");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->is_entry);
}

// 测试多次删除
TEST(TrieTest, MultipleDeletions) {
    Trie trie;
    
    trie.TrieInsert("cat");
    trie.TrieInsert("car");
    trie.TrieInsert("card");
    
    trie.TrieDelete("card");
    trie.TrieDelete("car");
    trie.TrieDelete("cat");
    
    EXPECT_EQ(trie.TrieSearch("card"), nullptr);
    EXPECT_EQ(trie.TrieSearch("car"), nullptr);
    EXPECT_EQ(trie.TrieSearch("cat"), nullptr);
}
