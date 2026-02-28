#pragma once

#include "Graph.hpp"
#include <vector>
#include <limits>
#include <utility>

// ── 结果结构 ─────────────────────────────────────────────────────────────────
struct DijkstraResult {
    std::vector<float> distance;   // 从源点到每个节点的最短距离
    std::vector<int>   last;       // 最短路径上的前驱节点（-1 = 无）
};

// ── Dijkstra 算法 ────────────────────────────────────────────────────────────
// 计算从 from_node_index 到 G 中所有其他节点的最短路径。
// 所有边的权重必须为非负数。
//
// ❶ Initialise distance[] = ∞, last[] = -1, unvisited = all nodes.
// ❷ While unvisited is not empty:
// ❸   Pick the unvisited node with minimal distance.
// ❹   For each edge of that node:
// ❺     Compute new_dist = distance[from] + edge.weight.
// ❻     If new_dist < distance[to], update.
inline DijkstraResult Dijkstras(const Graph& G, int from_node_index) {
    const float INF = std::numeric_limits<float>::infinity();

    // ❶
    std::vector<float> distance(G.num_nodes, INF);
    std::vector<int>   last(G.num_nodes, -1);
    std::vector<bool>  visited(G.num_nodes, false);
    distance[from_node_index] = 0.0f;

    // ❷
    for (int iter = 0; iter < G.num_nodes; iter++) {
        // ❸ 找到距离最小的未访问节点
        int next_index = -1;
        float best = INF;
        for (int i = 0; i < G.num_nodes; i++) {
            if (!visited[i] && distance[i] < best) {
                best = distance[i];
                next_index = i;
            }
        }
        if (next_index == -1) break;  // 剩余节点不可达

        visited[next_index] = true;
        const Node& current = G.nodes[next_index];

        // ❹ 松弛边
        for (const Edge& edge : current.edges) {
            // ❺
            float new_dist = distance[edge.from_node] + edge.weight;
            // ❻
            if (new_dist < distance[edge.to_node]) {
                distance[edge.to_node] = new_dist;
                last[edge.to_node]     = edge.from_node;
            }
        }
    }

    return {distance, last};
}

// ── 辅助函数：重建从源点到目标的最短路径 ─────────────────────────────────────
inline std::vector<int> DijkstraPath(const DijkstraResult& res, int target) {
    std::vector<int> path;
    if (res.distance[target] == std::numeric_limits<float>::infinity())
        return path;  // 不可达
    for (int v = target; v != -1; v = res.last[v])
        path.push_back(v);
    // 反转以获得源点 → 目标的顺序
    for (int i = 0, j = (int)path.size() - 1; i < j; i++, j--)
        std::swap(path[i], path[j]);
    return path;
}
