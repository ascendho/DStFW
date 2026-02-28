#pragma once

#include <string>
#include <vector>

// ── Edge ─────────────────────────────────────────────────────────────────────
struct Edge {
    int   to_node;
    int   from_node;
    float weight;

    Edge(int from, int to, float w = 1.0f)
        : to_node(to), from_node(from), weight(w) {}
};

// ── Node ─────────────────────────────────────────────────────────────────────
struct Node {
    std::string        name;
    int                id;
    std::vector<Edge>  edges;

    Node() : name(), id(-1) {}
    Node(const std::string& name, int id) : name(name), id(id) {}
};

// ── Graph (adjacency-list representation) ────────────────────────────────────
struct Graph {
    int                num_nodes;
    std::vector<Node>  nodes;

    explicit Graph(int n) : num_nodes(n), nodes(n) {
        for (int i = 0; i < n; i++) {
            nodes[i].id = i;
        }
    }

    // Convenience: name nodes
    void SetName(int id, const std::string& name) { nodes[id].name = name; }

    // Add a directed edge
    void AddDirectedEdge(int from, int to, float weight = 1.0f) {
        nodes[from].edges.emplace_back(from, to, weight);
    }

    // Add an undirected edge (two directed edges)
    void AddUndirectedEdge(int a, int b, float weight = 1.0f) {
        nodes[a].edges.emplace_back(a, b, weight);
        nodes[b].edges.emplace_back(b, a, weight);
    }
};
