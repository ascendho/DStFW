#pragma once

#include "Graph.hpp"
#include <vector>
#include <limits>
#include <utility>

// ── Result structure ─────────────────────────────────────────────────────────
struct PrimResult {
    std::vector<std::pair<int, int>> mst_edges;  // edges in the MST (from, to)
    float total_weight;                           // sum of MST edge weights
};

// ── Prim's Algorithm ─────────────────────────────────────────────────────────
// Finds a minimum-cost spanning tree of an undirected, connected graph.
//
// ❶ Initialise distance[] = ∞, last[] = -1, unvisited = all.
// ❷ While unvisited is not empty:
// ❸   Pick the unvisited node with minimal distance.
// ❹   If last[next] != -1, add that edge to MST.
// ❺   For each unvisited neighbour, update if edge.weight < distance[to].
inline PrimResult Prims(const Graph& G) {
    const float INF = std::numeric_limits<float>::infinity();

    // ❶
    std::vector<float> distance(G.num_nodes, INF);
    std::vector<int>   last(G.num_nodes, -1);
    std::vector<bool>  visited(G.num_nodes, false);

    std::vector<std::pair<int, int>> mst_edges;
    float total_weight = 0.0f;

    // Start from node 0 — give it distance 0 so it's picked first
    distance[0] = 0.0f;

    // ❷
    for (int iter = 0; iter < G.num_nodes; iter++) {
        // ❸ Find unvisited node with minimal distance
        int next_id = -1;
        float best = INF;
        for (int i = 0; i < G.num_nodes; i++) {
            if (!visited[i] && distance[i] < best) {
                best = distance[i];
                next_id = i;
            }
        }
        if (next_id == -1) break;  // disconnected component

        // ❹
        if (last[next_id] != -1) {
            mst_edges.emplace_back(last[next_id], next_id);
            total_weight += distance[next_id];
        }

        visited[next_id] = true;
        const Node& current = G.nodes[next_id];

        // ❺ Update unvisited neighbours
        for (const Edge& edge : current.edges) {
            if (!visited[edge.to_node] &&
                edge.weight < distance[edge.to_node]) {
                distance[edge.to_node] = edge.weight;
                last[edge.to_node]     = current.id;
            }
        }
    }

    return {mst_edges, total_weight};
}
