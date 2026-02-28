#pragma once

#include <cmath>
#include <limits>
#include <algorithm>

// ─── euclidean_dist ────────────────────────────────────────────────────────────
// euclidean_dist(Float: x1, Float: y1, Float: x2, Float: y2):
//     return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
inline float euclidean_dist(float x1, float y1, float x2, float y2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

// ─── MinDistToBin ─────────────────────────────────────────────────────────────
// MinDistToBin(Grid: g, Integer: xbin, Integer: ybin, Float: x, Float: y):
// ❶ IF xbin < 0 OR xbin >= g.num_x_bins: return Inf
//    IF ybin < 0 OR ybin >= g.num_y_bins: return Inf
// ❷ Compute x_min, x_max, x_dist for x dimension
// ❸ Compute y_min, y_max, y_dist for y dimension
//    return sqrt(x_dist*x_dist + y_dist*y_dist)
inline float MinDistToBin(const Grid& g, int xbin, int ybin, float x, float y) {
    static const float INF = std::numeric_limits<float>::infinity();

    // ❶ 验证单元格索引
    if (xbin < 0 || xbin >= g.num_x_bins) return INF;
    if (ybin < 0 || ybin >= g.num_y_bins) return INF;

    // ❷ x 维度距离
    float x_min = g.x_start + xbin       * g.x_bin_width;
    float x_max = g.x_start + (xbin + 1) * g.x_bin_width;
    float x_dist = 0.0f;
    if (x < x_min) x_dist = x_min - x;
    if (x > x_max) x_dist = x - x_max;

    // ❸ y 维度距离
    float y_min = g.y_start + ybin       * g.y_bin_width;
    float y_max = g.y_start + (ybin + 1) * g.y_bin_width;
    float y_dist = 0.0f;
    if (y < y_min) y_dist = y_min - y;
    if (y > y_max) y_dist = y - y_max;

    return std::sqrt(x_dist * x_dist + y_dist * y_dist);
}

// ─── GridLinearScanNN ─────────────────────────────────────────────────────────
// GridLinearScanNN(Grid: g, Float: x, Float: y):
// ❶ Float: best_dist = Inf; GridPoint: best_candidate = null
// ❷ WHILE xbin < g.num_x_bins:
//     WHILE ybin < g.num_y_bins:
// ❸       IF MinDistToBin(g, xbin, ybin, x, y) < best_dist:
// ❹           WHILE current != null:
// ❺               IF dist < best_dist: update best
// ❻ return best_candidate
inline GridPoint* GridLinearScanNN(const Grid& g, float x, float y) {
    static const float INF = std::numeric_limits<float>::infinity();

    // ❶ 初始化最佳跟踪
    float      best_dist      = INF;
    GridPoint* best_candidate = nullptr;

    int xbin = 0;
    // ❷ 遍历所有单元格
    while (xbin < g.num_x_bins) {
        int ybin = 0;
        while (ybin < g.num_y_bins) {
            // ❸ 剪枝不可能包含更近邻居的单元格
            if (MinDistToBin(g, xbin, ybin, x, y) < best_dist) {
                // ❹ 扫描单元格的链表
                GridPoint* current = g.bins[xbin][ybin];
                while (current != nullptr) {
                    float dist = euclidean_dist(x, y, current->x, current->y);
                    // ❺ 若更近则更新最佳结果
                    if (dist < best_dist) {
                        best_dist      = dist;
                        best_candidate = current;
                    }
                    current = current->next;
                }
            }
            ybin++;
        }
        xbin++;
    }
    // ❻ 返回最近邻（若网格为空则返回 nullptr）
    return best_candidate;
}

// ─── GridCheckBin ─────────────────────────────────────────────────────────────
// GridCheckBin(Grid: g, Integer: xbin, Integer: ybin,
//              Float: x, Float: y, Float: threshold):
// ❶ Check valid bin; return null if invalid
// ❷ best_dist = threshold (only consider points strictly closer)
// ❸ Iterate through linked list
// ❹ Compute dist; if dist < best_dist, update candidate
// ❺ return best_candidate
inline GridPoint* GridCheckBin(const Grid& g, int xbin, int ybin,
                                float x, float y, float threshold) {
    // ❶ 单元格索引安全检查
    if (xbin < 0 || xbin >= g.num_x_bins) return nullptr;
    if (ybin < 0 || ybin >= g.num_y_bins) return nullptr;

    GridPoint* best_candidate = nullptr;
    // ❷ 仅接受严格小于阈值距离的点
    float best_dist = threshold;

    // ❸ 扫描单元格
    GridPoint* current = g.bins[xbin][ybin];
    while (current != nullptr) {
        // ❹ 若更近则更新
        float dist = euclidean_dist(x, y, current->x, current->y);
        if (dist < best_dist) {
            best_dist      = dist;
            best_candidate = current;
        }
        current = current->next;
    }
    // ❺ 返回最近点（若没有比阈值更近的点则返回 nullptr）
    return best_candidate;
}

// ─── GridSearchExpanding ──────────────────────────────────────────────────────
// GridSearchExpanding(Grid: g, Float: x, Float: y):
// ❶ Find starting bin (xb, yb) – clamped to valid range
// ❷ WHILE explore: iterate steps = 0, 1, 2, ...
// ❸   For xoff in [-steps, steps]:
// ❹     yoff = steps - abs(xoff)
// ❺     Check (xb+xoff, yb-yoff) with MinDistToBin / GridCheckBin
// ❽     Check (xb+xoff, yb+yoff) if yoff != 0
// Terminate when no valid bin within current steps could hold a closer point
inline GridPoint* GridSearchExpanding(const Grid& g, float x, float y) {
    static const float INF = std::numeric_limits<float>::infinity();

    float      best_d  = INF;
    GridPoint* best_pt = nullptr;

    // ❶ 找到起始单元格，限制在有效范围内
    int xb = static_cast<int>(std::floor((x - g.x_start) / g.x_bin_width));
    if (xb < 0)                  xb = 0;
    if (xb >= g.num_x_bins)      xb = g.num_x_bins - 1;

    int yb = static_cast<int>(std::floor((y - g.y_start) / g.y_bin_width));
    if (yb < 0)                  yb = 0;
    if (yb >= g.num_y_bins)      yb = g.num_y_bins - 1;

    int  steps   = 0;
    bool explore = true;

    // ❷ 逐步向外扩展
    while (explore) {
        explore = false;

        // ❸ 遍历从 -steps 到 +steps 的 x 偏移量
        int xoff = -steps;
        while (xoff <= steps) {
            // ❹ y 方向的剩余步数（使用曼哈顿距离）
            int yoff = steps - std::abs(xoff);

            // ❺ 检查 (xb+xoff, yb-yoff) 处的单元格
            if (MinDistToBin(g, xb + xoff, yb - yoff, x, y) < best_d) {
                // ❻ 在该单元格中查找更近的点
                GridPoint* pt = GridCheckBin(g, xb + xoff, yb - yoff,
                                             x, y, best_d);
                if (pt != nullptr) {
                    best_d  = euclidean_dist(x, y, pt->x, pt->y);
                    best_pt = pt;
                }
                // ❼ 标记应继续探索
                explore = true;
            }

            // ❽ 仅当 yoff != 0 时检查 (xb+xoff, yb+yoff)（避免重复检查）
            if (yoff != 0 &&
                MinDistToBin(g, xb + xoff, yb + yoff, x, y) < best_d) {
                GridPoint* pt = GridCheckBin(g, xb + xoff, yb + yoff,
                                             x, y, best_d);
                if (pt != nullptr) {
                    best_d  = euclidean_dist(x, y, pt->x, pt->y);
                    best_pt = pt;
                }
                // ❾ 标记应继续探索
                explore = true;
            }

            xoff++;
        }
        steps++;
    }
    return best_pt;
}
