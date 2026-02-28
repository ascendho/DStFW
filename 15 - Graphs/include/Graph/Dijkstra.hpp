#pragma once

#include "Graph.hpp"
#include <vector>
#include <limits>
#include <utility>

// ── Result structure ─────────────────────────────────────────────────────────
struct DijkstraResult {
    std::vector<float> distance;   // shortest distance from source to each node
    std::vector<int>   last;       // predecessor on the shortest path (-1 = none)
};

// ── Dijkstra's Algorithm ─────────────────────────────────────────────────────
// Finds shortest paths from from_node_index to all other nodes in G.
// All edge weights must be non-negative.
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
        // ❸ Find unvisited node with minimal distance
        int next_index = -1;
        float best = INF;
        for (int i = 0; i < G.num_nodes; i++) {
            if (!visited[i] && distance[i] < best) {
                best = distance[i];
                next_index = i;
            }
        }
        if (next_index == -1) break;  // remaining nodes unreachable

        visited[next_index] = true;
        const Node& current = G.nodes[next_index];

        // ❹ Relax edges
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

// ── Helper: reconstruct shortest path from source to target ──────────────────
inline std::vector<int> DijkstraPath(const DijkstraResult& res, int target) {
    std::vector<int> path;
    if (res.distance[target] == std::numeric_limits<float>::infinity())
        return path;  // unreachable
    for (int v = target; v != -1; v = res.last[v])
        path.push_back(v);
    // Reverse to get source → target order
    for (int i = 0, j = (int)path.size() - 1; i < j; i++, j--)
        std::swap(path[i], path[j]);
    return path;
}
