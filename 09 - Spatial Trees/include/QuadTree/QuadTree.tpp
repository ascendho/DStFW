#pragma once

#include <cmath>
#include <algorithm>

// ─── QuadTreeNode 构造函数/析构函数 ──────────────────────────────────────────
inline QuadTreeNode::QuadTreeNode(float x_min_, float x_max_,
                                   float y_min_, float y_max_)
    : is_leaf(true), num_points(0),
      x_min(x_min_), x_max(x_max_),
      y_min(y_min_), y_max(y_max_)
{
    children[0][0] = children[0][1] = nullptr;
    children[1][0] = children[1][1] = nullptr;
}

inline QuadTreeNode::~QuadTreeNode() {
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            delete children[i][j];
            children[i][j] = nullptr;
        }
}

// ─── QuadTree 构造函数/析构函数 ──────────────────────────────────────────────
inline QuadTree::QuadTree(float x_min, float x_max, float y_min, float y_max)
    : root(new QuadTreeNode(x_min, x_max, y_min, y_max))
{}

inline QuadTree::~QuadTree() {
    delete root;
}

// ─── qt_approx_equal ──────────────────────────────────────────────────────────
// 复用第八章的 approx_equal 逻辑（代码清单 8-3）
static constexpr float QT_EPS = 1e-5f;

inline bool qt_approx_equal(float x1, float y1, float x2, float y2) {
    return (std::abs(x1 - x2) <= QT_EPS) && (std::abs(y1 - y2) <= QT_EPS);
}

// ─── qt_should_split ──────────────────────────────────────────────────────────
inline bool qt_should_split(const QuadTreeNode* node) {
    float x_half = (node->x_max - node->x_min) / 2.0f;
    float y_half = (node->y_max - node->y_min) / 2.0f;
    return (static_cast<int>(node->points.size()) > QUAD_MAX_LEAF_POINTS)
        && (x_half > QUAD_MIN_BIN_SIZE)
        && (y_half > QUAD_MIN_BIN_SIZE);
}

// ─── MinDist ──────────────────────────────────────────────────────────────────
// MinDist(QuadTreeNode: node, Float: x, Float: y):
//     Compute minimum Euclidean distance from (x,y) to any point in node's region.
inline float MinDist(const QuadTreeNode* node, float x, float y) {
    float x_dist = 0.0f;
    if (x < node->x_min) x_dist = node->x_min - x;
    if (x > node->x_max) x_dist = x - node->x_max;

    float y_dist = 0.0f;
    if (y < node->y_min) y_dist = node->y_min - y;
    if (y > node->y_max) y_dist = y - node->y_max;

    return std::sqrt(x_dist * x_dist + y_dist * y_dist);
}

// ─── QuadTreeNodeCollapse ─────────────────────────────────────────────────────
// QuadTreeNodeCollapse(QuadTreeNode: node):
// ❶ IF node.is_leaf: return node.points
// ❷ FOR i IN [0,1]: FOR j IN [0,1]:
//       IF node.children[i][j] != null:
//           sub_pts = QuadTreeNodeCollapse(node.children[i][j])
//           append sub_pts to node.points; set child = null
// ❸ node.is_leaf = True
// ❹ return node.points
inline std::vector<Point> QuadTreeNodeCollapse(QuadTreeNode* node) {
    // ❶ 已是叶节点：直接返回点集
    if (node->is_leaf) return node->points;

    // ❷ 从子节点收集所有点
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (node->children[i][j] != nullptr) {
                std::vector<Point> sub_pts =
                    QuadTreeNodeCollapse(node->children[i][j]);
                for (const auto& pt : sub_pts)
                    node->points.push_back(pt);
                delete node->children[i][j];
                node->children[i][j] = nullptr;
            }
        }
    }
    // ❸ 标记为叶节点
    node->is_leaf = true;
    // ❹ 返回收集的点
    return node->points;
}

// ─── QuadTreeInsert ───────────────────────────────────────────────────────────
// QuadTreeInsert(QuadTree: tree, Float: x, Float: y):
//     IF x < tree.root.x_min OR x > tree.root.x_max: return False
//     IF y < tree.root.y_min OR y > tree.root.y_max: return False
//     QuadTreeNodeInsert(tree.root, x, y); return True
inline bool QuadTreeInsert(QuadTree& tree, float x, float y) {
    if (x < tree.root->x_min || x > tree.root->x_max) return false;
    if (y < tree.root->y_min || y > tree.root->y_max) return false;
    QuadTreeNodeInsert(tree.root, x, y);
    return true;
}

// ─── QuadTreeNodeInsert ───────────────────────────────────────────────────────
// QuadTreeNodeInsert(QuadTreeNode: node, Float: x, Float: y):
// ❶ node.num_points++
// ❷ Compute x_bin_size, y_bin_size, xbin, ybin
// ❸ IF NOT node.is_leaf:
// ❹     IF node.children[xbin][ybin] == null: create child
//       QuadTreeNodeInsert(node.children[xbin][ybin], x, y); return
// ❺ node.points.append(Point(x, y))
// ❻ IF we satisfy conditions to split:
//       node.is_leaf = False
// ❼     FOR EACH pt IN node.points: QuadTreeNodeInsert(node, pt.x, pt.y)
// ❽     node.num_points -= length(node.points); node.points = []
inline void QuadTreeNodeInsert(QuadTreeNode* node, float x, float y) {
    // ❶ 递增总计数
    node->num_points++;

    // ❷ 确定子区间
    float x_bin_size = (node->x_max - node->x_min) / 2.0f;
    float y_bin_size = (node->y_max - node->y_min) / 2.0f;
    int xbin = static_cast<int>(std::floor((x - node->x_min) / x_bin_size));
    int ybin = static_cast<int>(std::floor((y - node->y_min) / y_bin_size));
    // 将值截断到 [0, 1] 以处理边界情况
    if (xbin < 0) xbin = 0;
    if (xbin > 1) xbin = 1;
    if (ybin < 0) ybin = 0;
    if (ybin > 1) ybin = 1;

    // ❸ 内部节点：递归进入正确的子节点
    if (!node->is_leaf) {
        // ❹ 如果子节点尚不存在则创建
        if (node->children[xbin][ybin] == nullptr) {
            node->children[xbin][ybin] = new QuadTreeNode(
                node->x_min + xbin       * x_bin_size,
                node->x_min + (xbin + 1) * x_bin_size,
                node->y_min + ybin       * y_bin_size,
                node->y_min + (ybin + 1) * y_bin_size);
        }
        QuadTreeNodeInsert(node->children[xbin][ybin], x, y);
        return;
    }

    // ❺ 叶节点：直接添加点
    node->points.push_back(Point(x, y));

    // ❻ 检查是否应该分割
    if (qt_should_split(node)) {
        node->is_leaf = false;

        // ❼ 将当前叶节点的所有点重新插入到现在的内部节点中
        std::vector<Point> old_points = node->points;
        node->points.clear();

        // ❽ 修正 num_points：下面的每次调用会再次递增已计数的
        //    相同点，因此减去该计数。
        node->num_points -= static_cast<int>(old_points.size());

        for (const auto& pt : old_points) {
            QuadTreeNodeInsert(node, pt.x, pt.y);
        }
    }
}

// ─── QuadTreeDelete ───────────────────────────────────────────────────────────
// QuadTreeDelete(QuadTree: tree, Float: x, Float: y):
//     IF x < root.x_min OR ... : return False
//     return QuadTreeNodeDelete(tree.root, x, y)
inline bool QuadTreeDelete(QuadTree& tree, float x, float y) {
    if (x < tree.root->x_min || x > tree.root->x_max) return false;
    if (y < tree.root->y_min || y > tree.root->y_max) return false;
    return QuadTreeNodeDelete(tree.root, x, y);
}

// ─── QuadTreeNodeDelete ───────────────────────────────────────────────────────
// QuadTreeNodeDelete(QuadTreeNode: node, Float: x, Float: y):
// ❶ IF node.is_leaf:
// ❷     WHILE i < length(node.points):
// ❸         IF approx_equal(points[i], (x,y)): remove, --num_points, return True
//         return False
// ❹ Compute xbin, ybin
// ❺ IF node.children[xbin][ybin] == null: return False
// ❻ IF QuadTreeNodeDelete(child, x, y):
//       --num_points
// ❼     IF child.num_points == 0: delete child
// ❽     IF no longer meets split: collapse
//       return True
// ❾ return False
inline bool QuadTreeNodeDelete(QuadTreeNode* node, float x, float y) {
    // ❶ 叶节点：搜索点列表
    if (node->is_leaf) {
        int i = 0;
        // ❷ 遍历点集
        while (i < static_cast<int>(node->points.size())) {
            // ❸ 近似相等检查
            if (qt_approx_equal(node->points[i].x, node->points[i].y, x, y)) {
                node->points.erase(node->points.begin() + i);
                node->num_points--;
                return true;
            }
            ++i;
        }
        return false;
    }

    // ❹ 内部节点：计算点应落入哪个子区间
    float x_bin_size = (node->x_max - node->x_min) / 2.0f;
    float y_bin_size = (node->y_max - node->y_min) / 2.0f;
    int xbin = static_cast<int>(std::floor((x - node->x_min) / x_bin_size));
    int ybin = static_cast<int>(std::floor((y - node->y_min) / y_bin_size));
    if (xbin < 0) xbin = 0;
    if (xbin > 1) xbin = 1;
    if (ybin < 0) ybin = 0;
    if (ybin > 1) ybin = 1;

    // ❺ 子节点不存在：点不在树中
    if (node->children[xbin][ybin] == nullptr) return false;

    // ❻ 递归进入子节点
    if (QuadTreeNodeDelete(node->children[xbin][ybin], x, y)) {
        node->num_points--;

        // ❼ 移除空子节点
        if (node->children[xbin][ybin]->num_points == 0) {
            delete node->children[xbin][ybin];
            node->children[xbin][ybin] = nullptr;
        }

        // ❽ 如果节点不再满足分割条件则合并
        if (node->num_points <= QUAD_MAX_LEAF_POINTS) {
            node->points = QuadTreeNodeCollapse(node);
        }
        return true;
    }
    // ❾ 在子树中未找到该点
    return false;
}

// ─── QuadTreeNearestNeighbor ──────────────────────────────────────────────────
// QuadTreeNearestNeighbor(QuadTree: tree, Float: x, Float: y):
//     return QuadTreeNodeNearestNeighbor(tree.root, x, y, Inf)
inline Point* QuadTreeNearestNeighbor(QuadTree& tree, float x, float y) {
    float best_dist = std::numeric_limits<float>::infinity();
    return QuadTreeNodeNearestNeighbor(tree.root, x, y, best_dist);
}

// ─── QuadTreeNodeNearestNeighbor ──────────────────────────────────────────────
// QuadTreeNodeNearestNeighbor(node, x, y, best_dist):
// ❶ IF MinDist(node, x, y) >= best_dist: return null
//    Point: best_candidate = null
// ❷ IF node.is_leaf:
//       FOR EACH current IN node.points:
//           dist = euclidean_dist(x, y, current.x, current.y)
//           IF dist < best_dist: best_dist = dist; best_candidate = current
//       return best_candidate
// ❸ Compute xbin, ybin (clamped)
// ❹ FOR EACH i IN [xbin, (xbin+1)%2]:
//       FOR EACH j IN [ybin, (ybin+1)%2]:
//           IF child != null: quad_best = recurse; IF quad_best != null: update
// ❺   return best_candidate
inline Point* QuadTreeNodeNearestNeighbor(QuadTreeNode* node, float x, float y,
                                           float& best_dist) {
    // ❶ 剪枝：节点距离太远
    if (MinDist(node, x, y) >= best_dist) return nullptr;

    Point* best_candidate = nullptr;

    // ❷ 叶节点：直接扫描所有点
    if (node->is_leaf) {
        for (auto& current : node->points) {
            float dx   = x - current.x;
            float dy   = y - current.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < best_dist) {
                best_dist      = dist;
                best_candidate = &current;
            }
        }
        return best_candidate;
    }

    // ❸ 内部节点：计算最近的子节点（xbin、ybin 取值 [0,1]）
    float x_bin_size = (node->x_max - node->x_min) / 2.0f;
    float y_bin_size = (node->y_max - node->y_min) / 2.0f;
    int xbin = static_cast<int>(std::floor((x - node->x_min) / x_bin_size));
    int ybin = static_cast<int>(std::floor((y - node->y_min) / y_bin_size));
    if (xbin < 0) xbin = 0;
    if (xbin > 1) xbin = 1;
    if (ybin < 0) ybin = 0;
    if (ybin > 1) ybin = 1;

    // ❹ 搜索子节点：先搜索最近的象限，然后搜索其他三个
    int xi_order[2] = { xbin, (xbin + 1) % 2 };
    int yi_order[2] = { ybin, (ybin + 1) % 2 };

    for (int i : xi_order) {
        for (int j : yi_order) {
            if (node->children[i][j] != nullptr) {
                Point* quad_best = QuadTreeNodeNearestNeighbor(
                    node->children[i][j], x, y, best_dist);
                // ❺ 如果找到更近的点则更新最优解
                if (quad_best != nullptr) {
                    best_candidate = quad_best;
                    // best_dist 已在递归调用中更新
                }
            }
        }
    }
    return best_candidate;
}
