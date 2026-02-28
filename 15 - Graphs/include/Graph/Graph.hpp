#pragma once

#include <string>
#include <vector>

// ── 边 ──────────────────────────────────────────────────────────────────────
struct Edge {
    int   to_node;
    int   from_node;
    float weight;

    Edge(int from, int to, float w = 1.0f)
        : to_node(to), from_node(from), weight(w) {}
};

// ── 节点 ─────────────────────────────────────────────────────────────────────
struct Node {
    std::string        name;
    int                id;
    std::vector<Edge>  edges;

    Node() : name(), id(-1) {}
    Node(const std::string& name, int id) : name(name), id(id) {}
};

// ── 图（邻接表表示）─────────────────────────────────────────────────────────
struct Graph {
    int                num_nodes;
    std::vector<Node>  nodes;

    explicit Graph(int n) : num_nodes(n), nodes(n) {
        for (int i = 0; i < n; i++) {
            nodes[i].id = i;
        }
    }

    // 便捷方法：为节点命名
    void SetName(int id, const std::string& name) { nodes[id].name = name; }

    // 添加有向边
    void AddDirectedEdge(int from, int to, float weight = 1.0f) {
        nodes[from].edges.emplace_back(from, to, weight);
    }

    // 添加无向边（两条有向边）
    void AddUndirectedEdge(int a, int b, float weight = 1.0f) {
        nodes[a].edges.emplace_back(a, b, weight);
        nodes[b].edges.emplace_back(b, a, weight);
    }
};
