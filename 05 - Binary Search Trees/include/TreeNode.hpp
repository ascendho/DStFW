#pragma once

template<typename T>
struct TreeNode {
    T value;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    
    explicit TreeNode(const T& val) 
        : value(val), left(nullptr), right(nullptr), parent(nullptr) {}
};
