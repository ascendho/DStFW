#pragma once

#include <vector>
#include <utility>
#include "KDTreeNode.hpp"

// ─── KD 树分割常量 ──────────────────────────────────────────────────────────
static constexpr int   KD_MAX_LEAF_POINTS = 1;
static constexpr float KD_MIN_WIDTH       = 1e-6f;

// ─── KD 距离辅助函数 ──────────────────────────────────────────────────────────
// KDTreeNodeMinDist(KDTreeNode: node, Point: pt):
//     Computes minimum Euclidean distance from pt to any point in node's bbox.
float KDTreeNodeMinDist(const KDTreeNode* node,
                        const std::vector<float>& pt);

// KD 树两个 N 维点之间的欧氏距离
float kd_euclidean_dist(const std::vector<float>& a,
                        const std::vector<float>& b);

// ─── 边界框辅助函数 ──────────────────────────────────────────────────────────
// ComputeBoundingBox(Array of Arrays: pts):
//     Returns (L, H) = per-dimension min/max arrays.
std::pair<std::vector<float>, std::vector<float>>
ComputeBoundingBox(const std::vector<std::vector<float>>& pts);

// ─── 构建 ─────────────────────────────────────────────────────────────────────
// BuildKDTree(KDTree: tree, Array of Arrays: pts):
//     Bulk-construct the k-d tree from a set of points.
bool BuildKDTree(KDTree& tree, std::vector<std::vector<float>> pts);

void RecursiveBuildKDTree(KDTreeNode* node, int num_dims,
                          std::vector<std::vector<float>> pts);

// ─── 插入 ─────────────────────────────────────────────────────────────────────
bool KDTreeInsert(KDTree& tree, const std::vector<float>& pt);
void KDTreeNodeInsert(KDTreeNode* node, const std::vector<float>& pt);

// ─── 删除 ─────────────────────────────────────────────────────────────────────
bool KDTreeDelete(KDTree& tree, const std::vector<float>& pt);
bool KDTreeNodeDelete(KDTreeNode* node, const std::vector<float>& pt);

// ─── 最近邻搜索 ──────────────────────────────────────────────────────────────
// KDTreeNearestNeighbor(KDTree: tree, Point: pt):
//     Returns pointer to nearest-neighbor point, or nullptr if tree is empty.
std::vector<float>* KDTreeNearestNeighbor(KDTree& tree,
                                          const std::vector<float>& pt);

std::vector<float>* KDTreeNodeNearestNeighbor(KDTreeNode* node,
                                              const std::vector<float>& pt,
                                              float& best_dist);

#include "KDTree.tpp"
