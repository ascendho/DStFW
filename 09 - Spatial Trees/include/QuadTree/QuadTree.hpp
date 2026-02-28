#pragma once

#include <vector>
#include <limits>
#include "Point.hpp"

// ─── 分割常量 ──────────────────────────────────────────────────────────────────
static constexpr int   QUAD_MAX_LEAF_POINTS = 1;
static constexpr float QUAD_MIN_BIN_SIZE    = 1e-6f;

// ─── QuadTreeNode ─────────────────────────────────────────────────────────────
// QuadTreeNode {
//     Boolean: is_leaf
//     Integer: num_points
//     Float: x_min, x_max, y_min, y_max
//     Matrix of QuadTreeNodes: children   (2x2)
//     Array of Points: points
// }
struct QuadTreeNode {
    bool  is_leaf;
    int   num_points;
    float x_min, x_max;
    float y_min, y_max;
    QuadTreeNode* children[2][2];
    std::vector<Point> points;

    QuadTreeNode(float x_min_, float x_max_, float y_min_, float y_max_);
    ~QuadTreeNode();
};

// ─── QuadTree ─────────────────────────────────────────────────────────────────
// QuadTree {
//     QuadTreeNode: root
// }
struct QuadTree {
    QuadTreeNode* root;

    QuadTree(float x_min, float x_max, float y_min, float y_max);
    ~QuadTree();
};

// ─── 辅助函数 ──────────────────────────────────────────────────────────────────
bool  qt_approx_equal(float x1, float y1, float x2, float y2);
bool  qt_should_split(const QuadTreeNode* node);
float MinDist(const QuadTreeNode* node, float x, float y);

// ─── 合并 ──────────────────────────────────────────────────────────────────────
std::vector<Point> QuadTreeNodeCollapse(QuadTreeNode* node);

// ─── 插入 ──────────────────────────────────────────────────────────────────────
bool QuadTreeInsert(QuadTree& tree, float x, float y);
void QuadTreeNodeInsert(QuadTreeNode* node, float x, float y);

// ─── 删除 ──────────────────────────────────────────────────────────────────────
bool QuadTreeDelete(QuadTree& tree, float x, float y);
bool QuadTreeNodeDelete(QuadTreeNode* node, float x, float y);

// ─── 最近邻搜索 ──────────────────────────────────────────────────────────────
Point* QuadTreeNearestNeighbor(QuadTree& tree, float x, float y);
Point* QuadTreeNodeNearestNeighbor(QuadTreeNode* node, float x, float y,
                                   float& best_dist);

#include "QuadTree.tpp"
