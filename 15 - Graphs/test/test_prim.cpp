#include <gtest/gtest.h>
#include "Graph/Graph.hpp"
#include "Graph/Prim.hpp"
#include <cmath>
#include <algorithm>
#include <set>

static bool ApproxEq(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) < eps;
}

// ── 构建书中的图（与 Dijkstra 测试相同）──────────────────────────────────────
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

// ── 测试 ─────────────────────────────────────────────────────────────────────

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
    // 0-1: 1, 1-2: 2, 0-2: 3 => 最小生成树选择边 0-1(1) 和 1-2(2) = 3
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

    // 最小生成树应有 7 条边（8 个节点）
    EXPECT_EQ(res.mst_edges.size(), 7u);

    // 最小生成树边（按权重）：D-F:0.3  D-E:0.4  A-C:0.5  A-D:0.5
    //                           C-E:0.5 还是 A-B？……计算预期权重。
    // 最优最小生成树：D-F(0.3) + D-E(0.4) + A-C(0.5) + A-D(0.5) + C-E(0.5)→已连通则跳过
    // Kruskal 排序：0.3(D-F) 0.4(D-E) 0.5(A-C) 0.5(A-D) 0.5(C-E)→跳过 0.9(F-H) 1.1(B-F) 1.5(A-B)→跳过 2.5(C-G)
    // {D,F} {D,E,F} {A,C,D,E,F}（A-D 连通）{A,C,D,E,F} + F-H: {A,C,D,E,F,H}
    // + B-F: {A,B,C,D,E,F,H} + C-G: 全部连通
    // Total = 0.3 + 0.4 + 0.5 + 0.5 + 0.9 + 1.1 + 2.5 = 6.2
    EXPECT_TRUE(ApproxEq(res.total_weight, 6.2f));
}

TEST(Prim, BookGraphMSTEdges) {
    Graph G = BookGraph();
    auto res = Prims(G);

    // 将最小生成树的边收集为无向边对 {min, max}
    std::set<std::pair<int, int>> edges;
    for (auto [a, b] : res.mst_edges) {
        edges.insert({std::min(a, b), std::max(a, b)});
    }

    // 预期的最小生成树边（参见上方权重计算）：
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
    // 两个连通分量：{0,1} 和 {2,3}
    Graph G(4);
    G.AddUndirectedEdge(0, 1, 1.0f);
    G.AddUndirectedEdge(2, 3, 2.0f);
    auto res = Prims(G);
    // Prim 从节点 0 开始只能覆盖 {0,1}
    EXPECT_EQ(res.mst_edges.size(), 1u);
    EXPECT_TRUE(ApproxEq(res.total_weight, 1.0f));
}
