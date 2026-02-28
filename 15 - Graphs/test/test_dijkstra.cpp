#include <gtest/gtest.h>
#include "Graph/Graph.hpp"
#include "Graph/Dijkstra.hpp"
#include <cmath>
#include <limits>
#include <vector>

static constexpr float INF = std::numeric_limits<float>::infinity();

// ── Helpers ──────────────────────────────────────────────────────────────────
static bool ApproxEq(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

// ── Build the book's weighted undirected graph (Figure 15-4) ─────────────────
//   Nodes: A(0) B(1) C(2) D(3) E(4) F(5) G(6) H(7)
//   Edges: A-B:1.5  A-C:0.5  A-D:0.5  B-F:1.1  C-E:0.5
//          C-G:2.5  D-E:0.4  D-F:0.3  F-H:0.9
static Graph BookGraph() {
    Graph G(8);
    G.SetName(0, "A"); G.SetName(1, "B"); G.SetName(2, "C");
    G.SetName(3, "D"); G.SetName(4, "E"); G.SetName(5, "F");
    G.SetName(6, "G"); G.SetName(7, "H");

    G.AddUndirectedEdge(0, 1, 1.5f);  // A-B
    G.AddUndirectedEdge(0, 2, 0.5f);  // A-C
    G.AddUndirectedEdge(0, 3, 0.5f);  // A-D
    G.AddUndirectedEdge(1, 5, 1.1f);  // B-F
    G.AddUndirectedEdge(2, 4, 0.5f);  // C-E
    G.AddUndirectedEdge(2, 6, 2.5f);  // C-G
    G.AddUndirectedEdge(3, 4, 0.4f);  // D-E
    G.AddUndirectedEdge(3, 5, 0.3f);  // D-F
    G.AddUndirectedEdge(5, 7, 0.9f);  // F-H
    return G;
}

// ── Tests ────────────────────────────────────────────────────────────────────

TEST(Dijkstra, SingleNode) {
    Graph G(1);
    auto res = Dijkstras(G, 0);
    EXPECT_TRUE(ApproxEq(res.distance[0], 0.0f));
    EXPECT_EQ(res.last[0], -1);
}

TEST(Dijkstra, TwoNodesDirected) {
    Graph G(2);
    G.AddDirectedEdge(0, 1, 3.0f);
    auto res = Dijkstras(G, 0);
    EXPECT_TRUE(ApproxEq(res.distance[0], 0.0f));
    EXPECT_TRUE(ApproxEq(res.distance[1], 3.0f));
    EXPECT_EQ(res.last[1], 0);
}

TEST(Dijkstra, UnreachableNode) {
    Graph G(3);
    G.AddDirectedEdge(0, 1, 1.0f);
    // Node 2 is unreachable
    auto res = Dijkstras(G, 0);
    EXPECT_TRUE(ApproxEq(res.distance[1], 1.0f));
    EXPECT_EQ(res.distance[2], INF);
}

TEST(Dijkstra, BookGraphDistancesFromA) {
    Graph G = BookGraph();
    auto res = Dijkstras(G, 0);

    // Expected shortest distances from A(0):
    // A=0  B=1.5  C=0.5  D=0.5  E=0.9  F=0.8  G=3.0  H=1.7
    EXPECT_TRUE(ApproxEq(res.distance[0], 0.0f));
    EXPECT_TRUE(ApproxEq(res.distance[1], 1.5f));
    EXPECT_TRUE(ApproxEq(res.distance[2], 0.5f));
    EXPECT_TRUE(ApproxEq(res.distance[3], 0.5f));
    EXPECT_TRUE(ApproxEq(res.distance[4], 0.9f));  // A→D→F = 0.5+0.3=0.8  A→C→E=1.0  A→D→E=0.9
    EXPECT_TRUE(ApproxEq(res.distance[5], 0.8f));  // A→D→F = 0.5+0.3=0.8
    EXPECT_TRUE(ApproxEq(res.distance[6], 3.0f));  // A→C→G = 0.5+2.5=3.0
    EXPECT_TRUE(ApproxEq(res.distance[7], 1.7f));  // A→D→F→H = 0.5+0.3+0.9=1.7
}

TEST(Dijkstra, BookGraphPathToH) {
    Graph G = BookGraph();
    auto res = Dijkstras(G, 0);
    auto path = DijkstraPath(res, 7);  // path to H
    // A(0) → D(3) → F(5) → H(7)
    ASSERT_EQ(path.size(), 4u);
    EXPECT_EQ(path[0], 0);
    EXPECT_EQ(path[1], 3);
    EXPECT_EQ(path[2], 5);
    EXPECT_EQ(path[3], 7);
}

TEST(Dijkstra, PathToUnreachable) {
    Graph G(3);
    G.AddDirectedEdge(0, 1, 1.0f);
    auto res = Dijkstras(G, 0);
    auto path = DijkstraPath(res, 2);
    EXPECT_TRUE(path.empty());
}

TEST(Dijkstra, PathToSelf) {
    Graph G(2);
    G.AddDirectedEdge(0, 1, 5.0f);
    auto res = Dijkstras(G, 0);
    auto path = DijkstraPath(res, 0);
    ASSERT_EQ(path.size(), 1u);
    EXPECT_EQ(path[0], 0);
}

TEST(Dijkstra, ChoosesShorterPath) {
    // 0 ->1  weight 10
    // 0 ->2  weight 1,  2 ->1 weight 2  (total 3, better)
    Graph G(3);
    G.AddDirectedEdge(0, 1, 10.0f);
    G.AddDirectedEdge(0, 2, 1.0f);
    G.AddDirectedEdge(2, 1, 2.0f);
    auto res = Dijkstras(G, 0);
    EXPECT_TRUE(ApproxEq(res.distance[1], 3.0f));
    auto path = DijkstraPath(res, 1);
    ASSERT_EQ(path.size(), 3u);
    EXPECT_EQ(path[0], 0);
    EXPECT_EQ(path[1], 2);
    EXPECT_EQ(path[2], 1);
}
