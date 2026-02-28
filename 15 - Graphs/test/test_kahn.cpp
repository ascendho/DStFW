#include <gtest/gtest.h>
#include "Graph/Graph.hpp"
#include "Graph/Kahn.hpp"
#include <vector>
#include <set>
#include <algorithm>

// ── Validate topological order ──────────────────────────────────────────────
// Every edge (u → v) must have u appearing before v in the sorted order.
static bool IsValidTopologicalOrder(const Graph& G, const std::vector<int>& order) {
    // Build position map
    std::vector<int> pos(G.num_nodes, -1);
    for (int i = 0; i < (int)order.size(); i++) {
        pos[order[i]] = i;
    }
    for (const Node& node : G.nodes) {
        if (pos[node.id] == -1) continue;  // not in order (cycle)
        for (const Edge& e : node.edges) {
            if (pos[e.to_node] == -1) return false;
            if (pos[node.id] >= pos[e.to_node]) return false;
        }
    }
    return true;
}

// ── Tests ────────────────────────────────────────────────────────────────────

TEST(Kahn, SingleNode) {
    Graph G(1);
    auto sorted = Kahns(G);
    ASSERT_EQ(sorted.size(), 1u);
    EXPECT_EQ(sorted[0], 0);
}

TEST(Kahn, LinearChain) {
    // 0 → 1 → 2 → 3
    Graph G(4);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(1, 2);
    G.AddDirectedEdge(2, 3);
    auto sorted = Kahns(G);
    ASSERT_EQ(sorted.size(), 4u);
    EXPECT_EQ(sorted[0], 0);
    EXPECT_EQ(sorted[1], 1);
    EXPECT_EQ(sorted[2], 2);
    EXPECT_EQ(sorted[3], 3);
}

TEST(Kahn, BookDAG) {
    // Figure 15-10: nodes A(0) B(1) C(2) D(3) E(4) F(5)
    // Edges: A→B, A→C, C→E, E→D, D→F, E→F
    Graph G(6);
    G.SetName(0, "A"); G.SetName(1, "B"); G.SetName(2, "C");
    G.SetName(3, "D"); G.SetName(4, "E"); G.SetName(5, "F");

    G.AddDirectedEdge(0, 1);  // A→B
    G.AddDirectedEdge(0, 2);  // A→C
    G.AddDirectedEdge(2, 4);  // C→E
    G.AddDirectedEdge(4, 3);  // E→D
    G.AddDirectedEdge(3, 5);  // D→F
    G.AddDirectedEdge(4, 5);  // E→F

    auto sorted = Kahns(G);
    ASSERT_EQ(sorted.size(), 6u);
    EXPECT_TRUE(IsValidTopologicalOrder(G, sorted));

    // A must come first (only source)
    EXPECT_EQ(sorted[0], 0);
}

TEST(Kahn, DiamondDAG) {
    //     0
    //    / \.
    //   1   2
    //    \ /
    //     3
    Graph G(4);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(0, 2);
    G.AddDirectedEdge(1, 3);
    G.AddDirectedEdge(2, 3);

    auto sorted = Kahns(G);
    ASSERT_EQ(sorted.size(), 4u);
    EXPECT_TRUE(IsValidTopologicalOrder(G, sorted));
    EXPECT_EQ(sorted[0], 0);
    EXPECT_EQ(sorted[3], 3);  // sink must be last
}

TEST(Kahn, CycleDetection) {
    // 0 → 1 → 2 → 0  (cycle)
    Graph G(3);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(1, 2);
    G.AddDirectedEdge(2, 0);

    auto sorted = Kahns(G);
    // Cycle means not all nodes can be sorted
    EXPECT_LT(sorted.size(), 3u);
}

TEST(Kahn, DisconnectedDAG) {
    // Two independent chains: 0→1  and  2→3
    Graph G(4);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(2, 3);

    auto sorted = Kahns(G);
    ASSERT_EQ(sorted.size(), 4u);
    EXPECT_TRUE(IsValidTopologicalOrder(G, sorted));
}

TEST(Kahn, NoEdges) {
    Graph G(3);
    auto sorted = Kahns(G);
    // All nodes are sources — any permutation is valid
    ASSERT_EQ(sorted.size(), 3u);
    std::set<int> ids(sorted.begin(), sorted.end());
    EXPECT_EQ(ids.size(), 3u);
}

TEST(Kahn, PartialCycle) {
    // 0 → 1 → 2 → 1  (cycle on 1,2), node 0 has no incoming
    Graph G(3);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(1, 2);
    G.AddDirectedEdge(2, 1);

    auto sorted = Kahns(G);
    // Only node 0 can be sorted; 1 and 2 are in a cycle
    EXPECT_EQ(sorted.size(), 1u);
    EXPECT_EQ(sorted[0], 0);
}
