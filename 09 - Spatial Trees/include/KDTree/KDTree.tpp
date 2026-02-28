#pragma once

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <limits>

// ─── KDTreeNode 构造函数/析构函数 ────────────────────────────────────────────
inline KDTreeNode::KDTreeNode()
    : is_leaf(true), num_dimensions(0), num_points(0),
      split_dim(-1), split_val(0.0f),
      left(nullptr), right(nullptr)
{}

inline KDTreeNode::~KDTreeNode() {
    delete left;
    delete right;
}

// ─── KDTree 构造函数/析构函数 ─────────────────────────────────────────────────
inline KDTree::KDTree(int num_dims)
    : num_dimensions(num_dims), root(nullptr)
{}

inline KDTree::~KDTree() {
    delete root;
}

// ─── kd_euclidean_dist ────────────────────────────────────────────────────────
inline float kd_euclidean_dist(const std::vector<float>& a,
                                const std::vector<float>& b) {
    float sum = 0.0f;
    for (int d = 0; d < static_cast<int>(a.size()); ++d) {
        float diff = a[d] - b[d];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

// ─── KDTreeNodeMinDist ────────────────────────────────────────────────────────
// KDTreeNodeMinDist(KDTreeNode: node, Point: pt):
//     dist_sum = 0; for each dim: diff=0 if inside, else edge dist; sum+=diff^2
//     return sqrt(dist_sum)
inline float KDTreeNodeMinDist(const KDTreeNode* node,
                                const std::vector<float>& pt) {
    float dist_sum = 0.0f;
    int d = 0;
    while (d < node->num_dimensions) {
        float diff = 0.0f;
        if (pt[d] < node->x_min[d])
            diff = node->x_min[d] - pt[d];
        if (pt[d] > node->x_max[d])
            diff = pt[d] - node->x_max[d];
        dist_sum += diff * diff;
        d++;
    }
    return std::sqrt(dist_sum);
}

// ─── ComputeBoundingBox ───────────────────────────────────────────────────────
// ComputeBoundingBox(Array of Arrays: pts):
// ❶ Check input not empty; extract num_dims
// ❷ Initialise L = H = pts[0]
// ❸ Seed L and H with first point
// ❹ Scan remaining points, expand bounds
// ❺ return (L, H)
inline std::pair<std::vector<float>, std::vector<float>>
ComputeBoundingBox(const std::vector<std::vector<float>>& pts) {
    // ❶ 验证输入
    int num_points = static_cast<int>(pts.size());
    if (num_points == 0) throw std::invalid_argument("ComputeBoundingBox: empty pts");
    int num_dims = static_cast<int>(pts[0].size());

    // ❷ 创建边界数组
    std::vector<float> L(num_dims), H(num_dims);

    // ❸ 用第一个点初始化
    int d = 0;
    while (d < num_dims) {
        L[d] = pts[0][d];
        H[d] = pts[0][d];
        d++;
    }

    // ❹ 用剩余点扩展边界
    int i = 1;
    while (i < num_points) {
        d = 0;
        while (d < num_dims) {
            if (L[d] > pts[i][d]) L[d] = pts[i][d];
            if (H[d] < pts[i][d]) H[d] = pts[i][d];
            d++;
        }
        i++;
    }

    // ❺ 返回边界
    return {L, H};
}

// ─── RecursiveBuildKDTree ─────────────────────────────────────────────────────
// RecursiveBuildKDTree(node, num_dims, pts):
// ❶ Fill bookkeeping fields; default to leaf
// ❷ Compute bounding box
// ❸ Find widest dimension (also track potential split_dim)
// ❹ If leaf conditions: store points and return
// ❺ Choose split_dim (widest), split_val (midpoint), is_leaf = False
// ❻ Partition points: left if pt[split_dim] <= split_val, else right
// ❼ Recursively build children
inline void RecursiveBuildKDTree(KDTreeNode* node, int num_dims,
                                  std::vector<std::vector<float>> pts) {
    // ❶ 簿记信息
    node->num_points     = static_cast<int>(pts.size());
    node->num_dimensions = num_dims;
    node->left           = nullptr;
    node->right          = nullptr;
    node->points.clear();
    node->split_dim = -1;
    node->split_val = 0.0f;
    node->is_leaf   = true;

    if (pts.empty()) return;

    // ❷ 计算紧密边界框
    auto [L, H] = ComputeBoundingBox(pts);
    node->x_min = L;
    node->x_max = H;

    // ❸ 查找最宽维度
    float max_width = 0.0f;
    int   split_dim = 0;
    int   d = 0;
    while (d < num_dims) {
        float w = node->x_max[d] - node->x_min[d];
        if (w > max_width) {
            max_width = w;
            split_dim = d;
        }
        d++;
    }

    // ❹ 叶节点条件：点太少或区间太窄
    if (node->num_points <= KD_MAX_LEAF_POINTS || max_width <= KD_MIN_WIDTH) {
        for (const auto& pt : pts)
            node->points.push_back(pt);
        return;
    }

    // ❺ 选择分割：最宽维度，中点值
    node->split_dim = split_dim;
    node->split_val = (node->x_min[split_dim] + node->x_max[split_dim]) / 2.0f;
    node->is_leaf   = false;

    // ❻ 划分点集
    std::vector<std::vector<float>> left_pts, right_pts;
    for (const auto& pt : pts) {
        if (pt[node->split_dim] <= node->split_val)
            left_pts.push_back(pt);
        else
            right_pts.push_back(pt);
    }

    // ❼ 递归构建子节点
    node->left = new KDTreeNode();
    RecursiveBuildKDTree(node->left, num_dims, left_pts);
    node->right = new KDTreeNode();
    RecursiveBuildKDTree(node->right, num_dims, right_pts);
}

// ─── BuildKDTree ─────────────────────────────────────────────────────────────
// BuildKDTree(KDTree: tree, Array of Arrays: pts):
//     Check dimensionality; allocate root; call RecursiveBuildKDTree
inline bool BuildKDTree(KDTree& tree,
                         std::vector<std::vector<float>> pts) {
    for (const auto& pt : pts) {
        if (static_cast<int>(pt.size()) != tree.num_dimensions) return false;
    }
    delete tree.root;
    if (!pts.empty()) {
        tree.root = new KDTreeNode();
        RecursiveBuildKDTree(tree.root, tree.num_dimensions, pts);
    } else {
        tree.root = nullptr;
    }
    return true;
}

// ─── KDTreeInsert ─────────────────────────────────────────────────────────────
inline bool KDTreeInsert(KDTree& tree, const std::vector<float>& pt) {
    if (static_cast<int>(pt.size()) != tree.num_dimensions) return false;
    if (tree.root == nullptr) {
        tree.root = new KDTreeNode();
        std::vector<std::vector<float>> pts = { pt };
        RecursiveBuildKDTree(tree.root, tree.num_dimensions, pts);
        return true;
    }
    KDTreeNodeInsert(tree.root, pt);
    return true;
}

// ─── KDTreeNodeInsert ──────────────────────────────────────────────────────────
// 插入：沿 split_dim/split_val 递归；更新边界框；满足条件时分割叶节点
inline void KDTreeNodeInsert(KDTreeNode* node, const std::vector<float>& pt) {
    // 更新边界框
    if (node->x_min.empty()) {
        node->x_min = pt;
        node->x_max = pt;
        node->num_dimensions = static_cast<int>(pt.size());
    } else {
        int d = 0;
        while (d < node->num_dimensions) {
            if (pt[d] < node->x_min[d]) node->x_min[d] = pt[d];
            if (pt[d] > node->x_max[d]) node->x_max[d] = pt[d];
            d++;
        }
    }
    node->num_points++;

    // 叶节点：添加点，满足条件则分割
    if (node->is_leaf) {
        node->points.push_back(pt);

        // 检查分割条件
        float max_width = 0.0f;
        int split_dim = 0;
        for (int d2 = 0; d2 < node->num_dimensions; ++d2) {
            float w = node->x_max[d2] - node->x_min[d2];
            if (w > max_width) { max_width = w; split_dim = d2; }
        }

        if (static_cast<int>(node->points.size()) > KD_MAX_LEAF_POINTS
            && max_width > KD_MIN_WIDTH) {
            // 从当前点重建此子树
            std::vector<std::vector<float>> old_pts = node->points;
            node->points.clear();
            node->is_leaf   = false;
            node->split_dim = split_dim;
            node->split_val = (node->x_min[split_dim] + node->x_max[split_dim]) / 2.0f;
            node->num_points = 0; // will be recomputed by children

            std::vector<std::vector<float>> left_pts, right_pts;
            for (const auto& p : old_pts) {
                if (p[node->split_dim] <= node->split_val) left_pts.push_back(p);
                else right_pts.push_back(p);
            }
            node->num_points = static_cast<int>(old_pts.size());

            node->left = new KDTreeNode();
            RecursiveBuildKDTree(node->left, node->num_dimensions, left_pts);
            node->right = new KDTreeNode();
            RecursiveBuildKDTree(node->right, node->num_dimensions, right_pts);
        }
        return;
    }

    // 内部节点：递归
    if (pt[node->split_dim] <= node->split_val) {
        if (node->left == nullptr) {
            node->left = new KDTreeNode();
            std::vector<std::vector<float>> pts = { pt };
            RecursiveBuildKDTree(node->left, node->num_dimensions, pts);
            return;
        }
        KDTreeNodeInsert(node->left, pt);
    } else {
        if (node->right == nullptr) {
            node->right = new KDTreeNode();
            std::vector<std::vector<float>> pts = { pt };
            RecursiveBuildKDTree(node->right, node->num_dimensions, pts);
            return;
        }
        KDTreeNodeInsert(node->right, pt);
    }
}

// ─── KDTreeDelete ─────────────────────────────────────────────────────────────
inline bool KDTreeDelete(KDTree& tree, const std::vector<float>& pt) {
    if (static_cast<int>(pt.size()) != tree.num_dimensions) return false;
    if (tree.root == nullptr) return false;
    return KDTreeNodeDelete(tree.root, pt);
}

// ─── KDTreeNodeDelete ──────────────────────────────────────────────────────────
// 删除：沿 split_dim/split_val 查找，从叶节点移除，更新边界框，合并
inline bool KDTreeNodeDelete(KDTreeNode* node, const std::vector<float>& pt) {
    if (node->is_leaf) {
        for (int i = 0; i < static_cast<int>(node->points.size()); ++i) {
            bool match = true;
            for (int d = 0; d < node->num_dimensions && match; ++d) {
                if (std::abs(node->points[i][d] - pt[d]) > 1e-5f) match = false;
            }
            if (match) {
                node->points.erase(node->points.begin() + i);
                node->num_points--;
                // 重新计算紧密边界框
                if (!node->points.empty()) {
                    auto [L, H] = ComputeBoundingBox(node->points);
                    node->x_min = L;
                    node->x_max = H;
                }
                return true;
            }
        }
        return false;
    }

    // 内部节点：决定进入哪个子节点
    bool found = false;
    if (pt[node->split_dim] <= node->split_val && node->left != nullptr) {
        found = KDTreeNodeDelete(node->left, pt);
    }
    if (!found && node->right != nullptr) {
        found = KDTreeNodeDelete(node->right, pt);
    }

    if (found) {
        node->num_points--;

        // 如果一个或两个子节点为空则合并
        bool left_empty  = (node->left  == nullptr || node->left->num_points  == 0);
        bool right_empty = (node->right == nullptr || node->right->num_points == 0);

        if (left_empty && right_empty) {
            delete node->left;  node->left  = nullptr;
            delete node->right; node->right = nullptr;
            node->is_leaf = true;
            node->points.clear();
        } else if (node->num_points <= KD_MAX_LEAF_POINTS) {
            // 收集所有剩余点并合并为叶节点
            std::vector<std::vector<float>> all;
            std::function<void(KDTreeNode*)> collect = [&](KDTreeNode* n) {
                if (n == nullptr) return;
                if (n->is_leaf) {
                    for (auto& p : n->points) all.push_back(p);
                } else {
                    collect(n->left);
                    collect(n->right);
                }
            };
            collect(node->left);
            collect(node->right);
            delete node->left;  node->left  = nullptr;
            delete node->right; node->right = nullptr;
            node->is_leaf   = true;
            node->points    = all;
            if (!all.empty()) {
                auto [L, H] = ComputeBoundingBox(all);
                node->x_min = L;
                node->x_max = H;
            }
        } else {
            // 从子节点收紧边界框
            if (node->left  && !node->left->x_min.empty())
                for (int d = 0; d < node->num_dimensions; ++d) {
                    node->x_min[d] = std::min(node->x_min[d], node->left->x_min[d]);
                    node->x_max[d] = std::max(node->x_max[d], node->left->x_max[d]);
                }
            if (node->right && !node->right->x_min.empty())
                for (int d = 0; d < node->num_dimensions; ++d) {
                    node->x_min[d] = std::min(node->x_min[d], node->right->x_min[d]);
                    node->x_max[d] = std::max(node->x_max[d], node->right->x_max[d]);
                }
        }
    }
    return found;
}

// ─── KDTreeNearestNeighbor ────────────────────────────────────────────────────
// KDTreeNearestNeighbor(KDTree: tree, Point: pt):
//     If root == null, return nullptr
//     best_dist = Inf; return KDTreeNodeNearestNeighbor(root, pt, best_dist)
inline std::vector<float>* KDTreeNearestNeighbor(KDTree& tree,
                                                   const std::vector<float>& pt) {
    if (tree.root == nullptr) return nullptr;
    float best_dist = std::numeric_limits<float>::infinity();
    return KDTreeNodeNearestNeighbor(tree.root, pt, best_dist);
}

// ─── KDTreeNodeNearestNeighbor ────────────────────────────────────────────────
// KDTreeNodeNearestNeighbor(node, pt, best_dist):
//     IF KDTreeNodeMinDist(node, pt) >= best_dist: return nullptr
//     IF leaf: scan points; return closest below best_dist
//     Recurse: closer child first, then other child; update best_dist
inline std::vector<float>* KDTreeNodeNearestNeighbor(KDTreeNode* node,
                                                       const std::vector<float>& pt,
                                                       float& best_dist) {
    if (node == nullptr) return nullptr;

    // 如果整个区域比当前最优距离更远则剪枝
    if (KDTreeNodeMinDist(node, pt) >= best_dist) return nullptr;

    std::vector<float>* best_candidate = nullptr;

    if (node->is_leaf) {
        for (auto& cur : node->points) {
            float dist = kd_euclidean_dist(cur, pt);
            if (dist < best_dist) {
                best_dist      = dist;
                best_candidate = const_cast<std::vector<float>*>(&cur);
            }
        }
        return best_candidate;
    }

    // 确定沿分割维度哪个子节点更接近目标点
    KDTreeNode* first  = nullptr;
    KDTreeNode* second = nullptr;
    if (node->left != nullptr || node->right != nullptr) {
        if (pt[node->split_dim] <= node->split_val) {
            first  = node->left;
            second = node->right;
        } else {
            first  = node->right;
            second = node->left;
        }
    }

    // 先搜索较近的子节点，然后搜索另一个
    auto* res1 = KDTreeNodeNearestNeighbor(first, pt, best_dist);
    if (res1) best_candidate = res1;

    auto* res2 = KDTreeNodeNearestNeighbor(second, pt, best_dist);
    if (res2) best_candidate = res2;

    return best_candidate;
}
