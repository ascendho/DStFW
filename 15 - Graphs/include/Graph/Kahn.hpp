#pragma once

#include "Graph.hpp"
#include <vector>
#include <stack>

// ── Kahn's Algorithm — Topological Sort ──────────────────────────────────────
// Works on directed acyclic graphs (DAGs).
// Returns a list of node indices in topological order.
// If the graph contains a cycle, the returned list will be shorter than
// num_nodes.
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

    // ❷ Count incoming edges
    for (const Node& node : G.nodes) {
        for (const Edge& edge : node.edges) {
            count[edge.to_node]++;
        }
    }

    // ❸ Seed with zero-incoming-edge nodes
    for (const Node& node : G.nodes) {
        if (count[node.id] == 0) {
            next.push(node.id);
        }
    }

    // ❹ Process
    while (!next.empty()) {
        int cur_id = next.top();
        next.pop();
        sorted.push_back(cur_id);

        // ❺ Remove outgoing edges
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
