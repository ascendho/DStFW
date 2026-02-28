#pragma once

#include <vector>

// KDTreeNode {
//     Boolean: is_leaf
//     Integer: num_dimensions
//     Integer: num_points
//     Array of Floats: x_min
//     Array of Floats: x_max
//     Integer: split_dim
//     Float: split_val
//     KDTreeNode: left
//     KDTreeNode: right
//     Array of Arrays: points
// }
struct KDTreeNode {
    bool  is_leaf;
    int   num_dimensions;
    int   num_points;
    std::vector<float> x_min;
    std::vector<float> x_max;
    int   split_dim;
    float split_val;
    KDTreeNode* left;
    KDTreeNode* right;
    std::vector<std::vector<float>> points;

    KDTreeNode();
    ~KDTreeNode();
};

// KDTree {
//     Integer: num_dimensions
//     KDTreeNode: root
// }
struct KDTree {
    int num_dimensions;
    KDTreeNode* root;

    KDTree(int num_dims);
    ~KDTree();
};
