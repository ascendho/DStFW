#include <gtest/gtest.h>
#include "Graph/Graph.hpp"
#include "Graph/Kahn.hpp"
#include <vector>
#include <set>
#include <algorithm>

// ── 验证拓扑顺序 ───────────────────────────────────────────────────────────
// 每条边 (u → v) 中，u 必须在排序结果中出现在 v 之前。
static bool IsValidTopologicalOrder(const Graph& G, const std::vector<int>& order) {
    // 构建位置映射
    std::vector<int> pos(G.num_nodes, -1);
    for (int i = 0; i < (int)order.size(); i++) {
        pos[order[i]] = i;
    }
    for (const Node& node : G.nodes) {
        if (pos[node.id] == -1) continue;  // 不在排序结果中（环）
        for (const Edge& e : node.edges) {
            if (pos[e.to_node] == -1) return false;
            if (pos[node.id] >= pos[e.to_node]) return false;
        }
    }
    return true;
}

// ── 测试 ─────────────────────────────────────────────────────────────────────

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
    // 图 15-10：节点 A(0) B(1) C(2) D(3) E(4) F(5)
    // 边：A→B, A→C, C→E, E→D, D→F, E→F
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

    // A 必须排在第一位（唯一的源点）
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
    // 0 → 1 → 2 → 0（环）
    Graph G(3);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(1, 2);
    G.AddDirectedEdge(2, 0);

    auto sorted = Kahns(G);
    // 存在环意味着不是所有节点都能被排序
    EXPECT_LT(sorted.size(), 3u);
}

TEST(Kahn, DisconnectedDAG) {
    // 两条独立的链：0→1 和 2→3
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
    // 所有节点都是源点——任何排列都有效
    ASSERT_EQ(sorted.size(), 3u);
    std::set<int> ids(sorted.begin(), sorted.end());
    EXPECT_EQ(ids.size(), 3u);
}

TEST(Kahn, PartialCycle) {
    // 0 → 1 → 2 → 1（1 和 2 构成环），节点 0 无入边
    Graph G(3);
    G.AddDirectedEdge(0, 1);
    G.AddDirectedEdge(1, 2);
    G.AddDirectedEdge(2, 1);

    auto sorted = Kahns(G);
    // 只有节点 0 能被排序；1 和 2 处于环中
    EXPECT_EQ(sorted.size(), 1u);
    EXPECT_EQ(sorted[0], 0);
}
