#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include "KDTree/KDTree.hpp"

// ─── 计算边界框 ──────────────────────────────────────────────────────────────
TEST(ComputeBoundingBoxTest, BasicTwoPoints) {
    std::vector<std::vector<float>> pts = {{1.0f,2.0f},{4.0f,0.5f}};
    auto [L, H] = ComputeBoundingBox(pts);
    EXPECT_FLOAT_EQ(L[0], 1.0f);
    EXPECT_FLOAT_EQ(H[0], 4.0f);
    EXPECT_FLOAT_EQ(L[1], 0.5f);
    EXPECT_FLOAT_EQ(H[1], 2.0f);
}

TEST(ComputeBoundingBoxTest, SinglePoint) {
    std::vector<std::vector<float>> pts = {{3.0f, 5.0f}};
    auto [L, H] = ComputeBoundingBox(pts);
    EXPECT_FLOAT_EQ(L[0], 3.0f);
    EXPECT_FLOAT_EQ(H[0], 3.0f);
}

TEST(ComputeBoundingBoxTest, ThreeDimensions) {
    std::vector<std::vector<float>> pts = {
        {1.0f, 2.0f, 3.0f},
        {5.0f, 0.0f, 4.0f},
        {2.0f, 3.0f, 1.0f}
    };
    auto [L, H] = ComputeBoundingBox(pts);
    EXPECT_FLOAT_EQ(L[0], 1.0f); EXPECT_FLOAT_EQ(H[0], 5.0f);
    EXPECT_FLOAT_EQ(L[1], 0.0f); EXPECT_FLOAT_EQ(H[1], 3.0f);
    EXPECT_FLOAT_EQ(L[2], 1.0f); EXPECT_FLOAT_EQ(H[2], 4.0f);
}

// ─── KD 树构建 ──────────────────────────────────────────────────────────────
TEST(KDTreeBuildTest, BuildEmpty) {
    KDTree tree(2);
    EXPECT_TRUE(BuildKDTree(tree, {}));
    EXPECT_EQ(tree.root, nullptr);
}

TEST(KDTreeBuildTest, BuildSinglePoint) {
    KDTree tree(2);
    EXPECT_TRUE(BuildKDTree(tree, {{1.0f, 2.0f}}));
    ASSERT_NE(tree.root, nullptr);
    EXPECT_EQ(tree.root->num_points, 1);
    EXPECT_TRUE(tree.root->is_leaf);
}

TEST(KDTreeBuildTest, BuildMultiplePoints) {
    KDTree tree(2);
    std::vector<std::vector<float>> pts = {
        {1.0f,1.0f},{3.0f,5.0f},{7.0f,2.0f},{4.0f,4.0f}
    };
    EXPECT_TRUE(BuildKDTree(tree, pts));
    ASSERT_NE(tree.root, nullptr);
    EXPECT_EQ(tree.root->num_points, 4);
}

TEST(KDTreeBuildTest, WrongDimensionalityRejected) {
    KDTree tree(2);
    EXPECT_FALSE(BuildKDTree(tree, {{1.0f, 2.0f, 3.0f}}));
}

// ─── KD 树插入 ──────────────────────────────────────────────────────────────
TEST(KDTreeInsertTest, InsertIntoEmpty) {
    KDTree tree(2);
    EXPECT_TRUE(KDTreeInsert(tree, {3.0f, 3.0f}));
    ASSERT_NE(tree.root, nullptr);
    EXPECT_EQ(tree.root->num_points, 1);
}

TEST(KDTreeInsertTest, InsertMultiple) {
    KDTree tree(2);
    KDTreeInsert(tree, {1.0f, 1.0f});
    KDTreeInsert(tree, {5.0f, 5.0f});
    KDTreeInsert(tree, {2.0f, 8.0f});
    EXPECT_EQ(tree.root->num_points, 3);
}

TEST(KDTreeInsertTest, WrongDimensionRejected) {
    KDTree tree(2);
    EXPECT_FALSE(KDTreeInsert(tree, {1.0f, 2.0f, 3.0f}));
}

// ─── KD 树删除 ──────────────────────────────────────────────────────────────
TEST(KDTreeDeleteTest, DeleteExistingPoint) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{5.0f,5.0f},{3.0f,3.0f}});
    EXPECT_TRUE(KDTreeDelete(tree, {5.0f, 5.0f}));
    EXPECT_EQ(tree.root->num_points, 2);
}

TEST(KDTreeDeleteTest, DeleteNonExistent) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{5.0f,5.0f}});
    EXPECT_FALSE(KDTreeDelete(tree, {9.0f, 9.0f}));
}

TEST(KDTreeDeleteTest, DeleteFromEmpty) {
    KDTree tree(2);
    EXPECT_FALSE(KDTreeDelete(tree, {1.0f, 1.0f}));
}

TEST(KDTreeDeleteTest, DeleteAllPoints) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{2.0f,2.0f}});
    EXPECT_TRUE(KDTreeDelete(tree, {1.0f, 1.0f}));
    EXPECT_TRUE(KDTreeDelete(tree, {2.0f, 2.0f}));
    EXPECT_EQ(tree.root->num_points, 0);
}

// ─── KD 树节点最小距离 ──────────────────────────────────────────────────────
TEST(KDTreeMinDistTest, PointInsideBBox) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{5.0f,5.0f}});
    // (3,3) 在边界框 [1,5]x[1,5] 内 → 距离 = 0
    EXPECT_FLOAT_EQ(KDTreeNodeMinDist(tree.root, {3.0f, 3.0f}), 0.0f);
}

TEST(KDTreeMinDistTest, PointOutsideBBox) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{3.0f,3.0f}});
    // 边界框为 [1,3]x[1,3]；点 (0,2) → x_dist=1, y_dist=0 → 距离=1
    EXPECT_FLOAT_EQ(KDTreeNodeMinDist(tree.root, {0.0f, 2.0f}), 1.0f);
}

// ─── KD 树最近邻 ──────────────────────────────────────────────────────────────
TEST(KDTreeNNTest, EmptyTreeReturnsNull) {
    KDTree tree(2);
    EXPECT_EQ(KDTreeNearestNeighbor(tree, {4.0f, 4.0f}), nullptr);
}

TEST(KDTreeNNTest, SinglePoint) {
    KDTree tree(2);
    BuildKDTree(tree, {{3.0f, 3.0f}});
    auto* nn = KDTreeNearestNeighbor(tree, {4.0f, 4.0f});
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ((*nn)[0], 3.0f);
    EXPECT_FLOAT_EQ((*nn)[1], 3.0f);
}

TEST(KDTreeNNTest, MultiplePoints2D) {
    KDTree tree(2);
    BuildKDTree(tree, {{1.0f,1.0f},{6.0f,6.0f},{4.0f,4.0f}});
    auto* nn = KDTreeNearestNeighbor(tree, {4.1f, 4.1f});
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ((*nn)[0], 4.0f);
    EXPECT_FLOAT_EQ((*nn)[1], 4.0f);
}

TEST(KDTreeNNTest, ThreeDimensional) {
    KDTree tree(3);
    BuildKDTree(tree, {
        {0.0f, 0.0f, 0.0f},
        {5.0f, 5.0f, 5.0f},
        {2.0f, 2.0f, 2.0f}
    });
    // 距离 (2.1, 2.1, 2.1) 最近的应该是 (2,2,2)
    auto* nn = KDTreeNearestNeighbor(tree, {2.1f, 2.1f, 2.1f});
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ((*nn)[0], 2.0f);
    EXPECT_FLOAT_EQ((*nn)[1], 2.0f);
    EXPECT_FLOAT_EQ((*nn)[2], 2.0f);
}

TEST(KDTreeNNTest, NNCorrectOnManyPoints) {
    KDTree tree(2);
    std::vector<std::vector<float>> pts = {
        {0.5f,0.5f},{2.3f,1.7f},{4.1f,6.2f},
        {7.8f,3.3f},{5.0f,5.0f},{9.1f,8.5f},
        {1.2f,9.0f},{6.6f,0.9f},{3.3f,7.7f}
    };
    BuildKDTree(tree, pts);

    // 对多个查询点进行暴力验证
    auto brute = [&](float qx, float qy) -> std::vector<float> {
        float best = std::numeric_limits<float>::infinity();
        std::vector<float> result;
        for (auto& p : pts) {
            float d = std::sqrt((p[0]-qx)*(p[0]-qx)+(p[1]-qy)*(p[1]-qy));
            if (d < best) { best = d; result = p; }
        }
        return result;
    };

    for (auto& [qx, qy] : std::vector<std::pair<float,float>>{
            {0.0f,0.0f},{5.0f,5.0f},{9.9f,9.9f},{3.0f,4.0f}}) {
        auto expected = brute(qx, qy);
        auto* nn = KDTreeNearestNeighbor(tree, {qx, qy});
        ASSERT_NE(nn, nullptr);
        float d_nn  = std::sqrt(((*nn)[0]-qx)*((*nn)[0]-qx)+((*nn)[1]-qy)*((*nn)[1]-qy));
        float d_exp = std::sqrt((expected[0]-qx)*(expected[0]-qx)+(expected[1]-qy)*(expected[1]-qy));
        EXPECT_FLOAT_EQ(d_nn, d_exp)
            << "Query (" << qx << "," << qy << "): expected dist "
            << d_exp << " got " << d_nn;
    }
}
