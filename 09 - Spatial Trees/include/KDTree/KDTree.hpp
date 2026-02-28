#pragma once

#include <vector>
#include <utility>
#include "KDTreeNode.hpp"

// ─── KDTree splitting constants ───────────────────────────────────────────────
static constexpr int   KD_MAX_LEAF_POINTS = 1;
static constexpr float KD_MIN_WIDTH       = 1e-6f;

// ─── KD-distance helper ───────────────────────────────────────────────────────
// KDTreeNodeMinDist(KDTreeNode: node, Point: pt):
//     Computes minimum Euclidean distance from pt to any point in node's bbox.
float KDTreeNodeMinDist(const KDTreeNode* node,
                        const std::vector<float>& pt);

// KD-tree Euclidean distance between two N-dimensional points
float kd_euclidean_dist(const std::vector<float>& a,
                        const std::vector<float>& b);

// ─── Bounding box helper ──────────────────────────────────────────────────────
// ComputeBoundingBox(Array of Arrays: pts):
//     Returns (L, H) = per-dimension min/max arrays.
std::pair<std::vector<float>, std::vector<float>>
ComputeBoundingBox(const std::vector<std::vector<float>>& pts);

// ─── Build ────────────────────────────────────────────────────────────────────
// BuildKDTree(KDTree: tree, Array of Arrays: pts):
//     Bulk-construct the k-d tree from a set of points.
bool BuildKDTree(KDTree& tree, std::vector<std::vector<float>> pts);

void RecursiveBuildKDTree(KDTreeNode* node, int num_dims,
                          std::vector<std::vector<float>> pts);

// ─── Insert ───────────────────────────────────────────────────────────────────
bool KDTreeInsert(KDTree& tree, const std::vector<float>& pt);
void KDTreeNodeInsert(KDTreeNode* node, const std::vector<float>& pt);

// ─── Delete ───────────────────────────────────────────────────────────────────
bool KDTreeDelete(KDTree& tree, const std::vector<float>& pt);
bool KDTreeNodeDelete(KDTreeNode* node, const std::vector<float>& pt);

// ─── Nearest-Neighbor Search ──────────────────────────────────────────────────
// KDTreeNearestNeighbor(KDTree: tree, Point: pt):
//     Returns pointer to nearest-neighbor point, or nullptr if tree is empty.
std::vector<float>* KDTreeNearestNeighbor(KDTree& tree,
                                          const std::vector<float>& pt);

std::vector<float>* KDTreeNodeNearestNeighbor(KDTreeNode* node,
                                              const std::vector<float>& pt,
                                              float& best_dist);

#include "KDTree.tpp"
