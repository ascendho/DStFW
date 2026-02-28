#pragma once

#include "Graph.hpp"
#include <vector>
#include <stack>

// ── Kahn 算法——拓扑排序 ──────────────────────────────────────────────────────
// 适用于有向无环图（DAG）。
// 返回按拓扑顺序排列的节点索引列表。
// 若图中包含环，返回的列表长度将小于 num_nodes。
//
// ❶ Create sorted[], count[], and a stack next.
// ❷ Count incoming edges for every node.
// ❸ Push all nodes with count == 0 onto next.
// ❹ While next is not empty:
//      Pop a node, append to sorted.
// ❺    For each outgoing edge, decrement the destination's count.
// ❻    If count reaches 0, push the destination onto next.
inline std::vector<int> Kahns(const Graph& G) {
    // ❶
    std::vector<int> sorted;
    sorted.reserve(G.num_nodes);
    std::vector<int> count(G.num_nodes, 0);
    std::stack<int> next;

    // ❷ 统计入度
    for (const Node& node : G.nodes) {
        for (const Edge& edge : node.edges) {
            count[edge.to_node]++;
        }
    }

    // ❸ 将入度为零的节点入栈
    for (const Node& node : G.nodes) {
        if (count[node.id] == 0) {
            next.push(node.id);
        }
    }

    // ❹ 处理
    while (!next.empty()) {
        int cur_id = next.top();
        next.pop();
        sorted.push_back(cur_id);

        // ❺ 移除出边
        for (const Edge& edge : G.nodes[cur_id].edges) {
            count[edge.to_node]--;
            // ❻
            if (count[edge.to_node] == 0) {
                next.push(edge.to_node);
            }
        }
    }

    return sorted;
}
