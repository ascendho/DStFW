#include <gtest/gtest.h>
#include "Graph/Graph.hpp"
#include "Graph/Prim.hpp"
#include <cmath>
#include <algorithm>
#include <set>

static bool ApproxEq(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

// ── Build the book's graph (same as Dijkstra tests) ─────────────────────────
static Graph BookGraph() {
    Graph G(8);
    G.SetName(0, "A"); G.SetName(1, "B"); G.SetName(2, "C");
    G.SetName(3, "D"); G.SetName(4, "E"); G.SetName(5, "F");
    G.SetName(6, "G"); G.SetName(7, "H");

    G.AddUndirectedEdge(0, 1, 1.5f);
    G.AddUndirectedEdge(0, 2, 0.5f);
    G.AddUndirectedEdge(0, 3, 0.5f);
    G.AddUndirectedEdge(1, 5, 1.1f);
    G.AddUndirectedEdge(2, 4, 0.5f);
    G.AddUndirectedEdge(2, 6, 2.5f);
    G.AddUndirectedEdge(3, 4, 0.4f);
    G.AddUndirectedEdge(3, 5, 0.3f);
    G.AddUndirectedEdge(5, 7, 0.9f);
    return G;
}

// ── Tests ────────────────────────────────────────────────────────────────────

TEST(Prim, SingleNode) {
    Graph G(1);
    auto res = Prims(G);
    EXPECT_TRUE(res.mst_edges.empty());
    EXPECT_TRUE(ApproxEq(res.total_weight, 0.0f));
}

TEST(Prim, TwoNodes) {
    Graph G(2);
    G.AddUndirectedEdge(0, 1, 4.5f);
    auto res = Prims(G);
    ASSERT_EQ(res.mst_edges.size(), 1u);
    EXPECT_TRUE(ApproxEq(res.total_weight, 4.5f));
}

TEST(Prim, Triangle) {
    // 0-1: 1, 1-2: 2, 0-2: 3   => MST picks edges 0-1(1) and 1-2(2) = 3
    Graph G(3);
    G.AddUndirectedEdge(0, 1, 1.0f);
    G.AddUndirectedEdge(1, 2, 2.0f);
    G.AddUndirectedEdge(0, 2, 3.0f);
    auto res = Prims(G);
    EXPECT_EQ(res.mst_edges.size(), 2u);
    EXPECT_TRUE(ApproxEq(res.total_weight, 3.0f));
}

TEST(Prim, BookGraphMSTWeight) {
    Graph G = BookGraph();
    auto res = Prims(G);

    // MST should have 7 edges for 8 nodes
    EXPECT_EQ(res.mst_edges.size(), 7u);

    // MST edges (by weight):  D-F:0.3  D-E:0.4  A-C:0.5  A-D:0.5
    //                          C-E:0.5 or A-B? ... Let's compute expected weight.
    // Optimal MST: D-F(0.3) + D-E(0.4) + A-C(0.5) + A-D(0.5) + C-E(0.5)→skip if already connected
    // Kruskal ordering: 0.3(D-F) 0.4(D-E) 0.5(A-C) 0.5(A-D) 0.5(C-E)→skip 0.9(F-H) 1.1(B-F) 1.5(A-B)→skip 2.5(C-G)
    // {D,F} {D,E,F} {A,C,D,E,F} (A-D connects) {A,C,D,E,F} + F-H: {A,C,D,E,F,H}
    // + B-F: {A,B,C,D,E,F,H} + C-G: all connected
    // Total = 0.3 + 0.4 + 0.5 + 0.5 + 0.9 + 1.1 + 2.5 = 6.2
    EXPECT_TRUE(ApproxEq(res.total_weight, 6.2f));
}

TEST(Prim, BookGraphMSTEdges) {
    Graph G = BookGraph();
    auto res = Prims(G);

    // Collect MST edges as undirected pairs {min, max}
    std::set<std::pair<int, int>> edges;
    for (auto [a, b] : res.mst_edges) {
        edges.insert({std::min(a, b), std::max(a, b)});
    }

    // Expected MST edges (see weight calculation above):
    // {0,2}=A-C  {0,3}=A-D  {3,5}=D-F  {3,4}=D-E  {5,7}=F-H  {1,5}=B-F  {2,6}=C-G
    EXPECT_TRUE(edges.count({0, 2}));  // A-C 0.5
    EXPECT_TRUE(edges.count({0, 3}));  // A-D 0.5
    EXPECT_TRUE(edges.count({3, 5}));  // D-F 0.3
    EXPECT_TRUE(edges.count({3, 4}));  // D-E 0.4
    EXPECT_TRUE(edges.count({5, 7}));  // F-H 0.9
    EXPECT_TRUE(edges.count({1, 5}));  // B-F 1.1
    EXPECT_TRUE(edges.count({2, 6}));  // C-G 2.5
}

TEST(Prim, DisconnectedPartial) {
    // Two components: {0,1} and {2,3}
    Graph G(4);
    G.AddUndirectedEdge(0, 1, 1.0f);
    G.AddUndirectedEdge(2, 3, 2.0f);
    auto res = Prims(G);
    // Prim starting from 0 will only span {0,1}
    EXPECT_EQ(res.mst_edges.size(), 1u);
    EXPECT_TRUE(ApproxEq(res.total_weight, 1.0f));
}
