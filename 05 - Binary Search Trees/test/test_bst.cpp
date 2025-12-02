#include <gtest/gtest.h>
#include "BinarySearchTree.hpp"

// 测试基本插入和搜索
TEST(BinarySearchTreeTest, InsertAndFindValue) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    tree.InsertTreeNode(40);
    tree.InsertTreeNode(60);
    tree.InsertTreeNode(80);
    
    TreeNode<int>* node = tree.FindTreeNode(50);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 50);
    
    node = tree.FindTreeNode(30);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 30);
    
    node = tree.FindTreeNode(70);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 70);
    
    node = tree.FindTreeNode(100);
    EXPECT_EQ(node, nullptr);
}

// 测试迭代搜索
TEST(BinarySearchTreeTest, IterativeSearch) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    tree.InsertTreeNode(40);
    
    TreeNode<int>* node = tree.FindValueItr(tree.root, 30);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 30);
    
    node = tree.FindValueItr(tree.root, 100);
    EXPECT_EQ(node, nullptr);
}

// 测试空树
TEST(BinarySearchTreeTest, EmptyTree) {
    BinarySearchTree<int> tree;
    
    TreeNode<int>* node = tree.FindTreeNode(50);
    EXPECT_EQ(node, nullptr);
}

// 测试删除叶子节点
TEST(BinarySearchTreeTest, RemoveLeafNode) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    
    TreeNode<int>* node = tree.FindTreeNode(20);
    ASSERT_NE(node, nullptr);
    
    tree.RemoveTreeNode(node);
    
    node = tree.FindTreeNode(20);
    EXPECT_EQ(node, nullptr);
    
    // 验证其他节点仍然存在
    EXPECT_NE(tree.FindTreeNode(30), nullptr);
    EXPECT_NE(tree.FindTreeNode(50), nullptr);
}

// 测试删除只有一个子节点的节点
TEST(BinarySearchTreeTest, RemoveNodeWithOneChild) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    tree.InsertTreeNode(10);
    
    TreeNode<int>* node = tree.FindTreeNode(20);
    ASSERT_NE(node, nullptr);
    
    tree.RemoveTreeNode(node);
    
    node = tree.FindTreeNode(20);
    EXPECT_EQ(node, nullptr);
    
    // 验证子节点仍然存在
    EXPECT_NE(tree.FindTreeNode(10), nullptr);
    EXPECT_NE(tree.FindTreeNode(30), nullptr);
}

// 测试删除有两个子节点的节点
TEST(BinarySearchTreeTest, RemoveNodeWithTwoChildren) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    tree.InsertTreeNode(40);
    tree.InsertTreeNode(60);
    tree.InsertTreeNode(80);
    
    TreeNode<int>* node = tree.FindTreeNode(30);
    ASSERT_NE(node, nullptr);
    
    tree.RemoveTreeNode(node);
    
    node = tree.FindTreeNode(30);
    EXPECT_EQ(node, nullptr);
    
    // 验证子节点仍然存在
    EXPECT_NE(tree.FindTreeNode(20), nullptr);
    EXPECT_NE(tree.FindTreeNode(40), nullptr);
}

// 测试删除根节点
TEST(BinarySearchTreeTest, RemoveRoot) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    
    TreeNode<int>* node = tree.FindTreeNode(50);
    ASSERT_NE(node, nullptr);
    
    tree.RemoveTreeNode(node);
    
    node = tree.FindTreeNode(50);
    EXPECT_EQ(node, nullptr);
    
    // 验证其他节点仍然存在
    EXPECT_NE(tree.FindTreeNode(30), nullptr);
    EXPECT_NE(tree.FindTreeNode(70), nullptr);
}

// 测试树结构
TEST(BinarySearchTreeTest, TreeStructure) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(30);
    tree.InsertTreeNode(70);
    tree.InsertTreeNode(20);
    tree.InsertTreeNode(40);
    
    ASSERT_NE(tree.root, nullptr);
    EXPECT_EQ(tree.root->value, 50);
    
    ASSERT_NE(tree.root->left, nullptr);
    EXPECT_EQ(tree.root->left->value, 30);
    
    ASSERT_NE(tree.root->right, nullptr);
    EXPECT_EQ(tree.root->right->value, 70);
    
    ASSERT_NE(tree.root->left->left, nullptr);
    EXPECT_EQ(tree.root->left->left->value, 20);
    
    ASSERT_NE(tree.root->left->right, nullptr);
    EXPECT_EQ(tree.root->left->right->value, 40);
}

// 测试字符串类型
TEST(BinarySearchTreeTest, StringTree) {
    BinarySearchTree<std::string> tree;
    
    tree.InsertTreeNode("dog");
    tree.InsertTreeNode("cat");
    tree.InsertTreeNode("elephant");
    tree.InsertTreeNode("bear");
    
    TreeNode<std::string>* node = tree.FindTreeNode("dog");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, "dog");
    
    node = tree.FindTreeNode("cat");
    ASSERT_NE(node, nullptr);
    
    node = tree.FindTreeNode("lion");
    EXPECT_EQ(node, nullptr);
}

// 测试重复插入
TEST(BinarySearchTreeTest, DuplicateInsertion) {
    BinarySearchTree<int> tree;
    
    tree.InsertTreeNode(50);
    tree.InsertTreeNode(50);
    
    TreeNode<int>* node = tree.FindTreeNode(50);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->value, 50);
    
    // 应该仍然只有一个值为50的节点
    EXPECT_EQ(tree.root->left, nullptr);
    EXPECT_EQ(tree.root->right, nullptr);
}
