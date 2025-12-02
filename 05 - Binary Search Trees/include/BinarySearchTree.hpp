#pragma once
#include <concepts>
#include "TreeNode.hpp"

template<typename T>
requires std::totally_ordered<T>
class BinarySearchTree {
public:
    TreeNode<T>* root;
    
    BinarySearchTree() : root(nullptr) {}
    
    ~BinarySearchTree() {
        DeleteTree(root);
    }
    
    // 搜索函数
    TreeNode<T>* FindValue(TreeNode<T>* current, const T& target);
    TreeNode<T>* FindValueItr(TreeNode<T>* root_node, const T& target);
    TreeNode<T>* FindTreeNode(const T& target);
    
    // 插入函数
    void InsertNode(TreeNode<T>* current, const T& new_value);
    void InsertTreeNode(const T& new_value);
    
    // 删除函数
    void RemoveTreeNode(TreeNode<T>* node);
    
private:
    void DeleteTree(TreeNode<T>* node) {
        if (node == nullptr) return;
        DeleteTree(node->left);
        DeleteTree(node->right);
        delete node;
    }
};

#include "BinarySearchTree.tpp"
