#include <gtest/gtest.h>
#include <cmath>
#include "QuadTree/QuadTree.hpp"

// ─── Helper ───────────────────────────────────────────────────────────────────
static bool qt_tree_contains(const QuadTree& tree, float x, float y) {
    std::function<bool(const QuadTreeNode*)> find = [&](const QuadTreeNode* n) {
        if (n == nullptr) return false;
        if (n->is_leaf) {
            for (const auto& p : n->points)
                if (qt_approx_equal(p.x, p.y, x, y)) return true;
            return false;
        }
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                if (find(n->children[i][j])) return true;
        return false;
    };
    return find(tree.root);
}

// ─── Insert ───────────────────────────────────────────────────────────────────
TEST(QuadTreeInsertTest, InsertSinglePoint) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_TRUE(QuadTreeInsert(tree, 3.0f, 3.0f));
    EXPECT_EQ(tree.root->num_points, 1);
}

TEST(QuadTreeInsertTest, InsertOutOfBounds) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_FALSE(QuadTreeInsert(tree, -1.0f, 4.0f));
    EXPECT_FALSE(QuadTreeInsert(tree, 4.0f, 9.0f));
    EXPECT_EQ(tree.root->num_points, 0);
}

TEST(QuadTreeInsertTest, InsertMultiplePointsCausingSplit) {
    // QUAD_MAX_LEAF_POINTS = 1, so two distinct points force a split
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    QuadTreeInsert(tree, 6.0f, 6.0f);
    // Root should now be internal
    EXPECT_FALSE(tree.root->is_leaf);
    EXPECT_EQ(tree.root->num_points, 2);
}

TEST(QuadTreeInsertTest, InsertManyPoints) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    std::vector<std::pair<float,float>> pts = {
        {1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7}
    };
    for (auto& [x, y] : pts) QuadTreeInsert(tree, x, y);
    EXPECT_EQ(tree.root->num_points, 7);
}

TEST(QuadTreeInsertTest, NumPointsTrackedCorrectly) {
    QuadTree tree(0.0f, 10.0f, 0.0f, 10.0f);
    for (int i = 1; i <= 5; ++i)
        QuadTreeInsert(tree, static_cast<float>(i), static_cast<float>(i));
    EXPECT_EQ(tree.root->num_points, 5);
}

// ─── Delete ───────────────────────────────────────────────────────────────────
TEST(QuadTreeDeleteTest, DeleteExistingPoint) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    EXPECT_TRUE(QuadTreeDelete(tree, 1.0f, 1.0f));
    EXPECT_EQ(tree.root->num_points, 0);
}

TEST(QuadTreeDeleteTest, DeleteNonExistentPoint) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    EXPECT_FALSE(QuadTreeDelete(tree, 5.0f, 5.0f));
    EXPECT_EQ(tree.root->num_points, 1);
}

TEST(QuadTreeDeleteTest, DeleteOutOfBoundsPoint) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_FALSE(QuadTreeDelete(tree, -1.0f, 0.0f));
}

TEST(QuadTreeDeleteTest, DeleteTriggersMerge) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    QuadTreeInsert(tree, 6.0f, 6.0f);
    // Should have split the root
    EXPECT_FALSE(tree.root->is_leaf);
    // Delete one - with 1 remaining point, root should collapse back to leaf
    EXPECT_TRUE(QuadTreeDelete(tree, 6.0f, 6.0f));
    EXPECT_TRUE(tree.root->is_leaf);
    EXPECT_EQ(tree.root->num_points, 1);
}

TEST(QuadTreeDeleteTest, DeleteOnlyRemovesFirstDuplicate) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    EXPECT_TRUE(QuadTreeDelete(tree, 1.0f, 1.0f));
    EXPECT_EQ(tree.root->num_points, 1);
    EXPECT_TRUE(QuadTreeDelete(tree, 1.0f, 1.0f));
    EXPECT_EQ(tree.root->num_points, 0);
}

// ─── Nearest-Neighbor Search ──────────────────────────────────────────────────
TEST(QuadTreeNNTest, EmptyTreeReturnsNull) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_EQ(QuadTreeNearestNeighbor(tree, 4.0f, 4.0f), nullptr);
}

TEST(QuadTreeNNTest, SinglePoint) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 3.0f, 3.0f);
    Point* nn = QuadTreeNearestNeighbor(tree, 4.0f, 4.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 3.0f);
    EXPECT_FLOAT_EQ(nn->y, 3.0f);
}

TEST(QuadTreeNNTest, MultiplePoints) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    QuadTreeInsert(tree, 6.0f, 6.0f);
    QuadTreeInsert(tree, 4.0f, 4.0f);
    Point* nn = QuadTreeNearestNeighbor(tree, 4.1f, 4.1f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 4.0f);
    EXPECT_FLOAT_EQ(nn->y, 4.0f);
}

TEST(QuadTreeNNTest, TargetOutsideTree) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 7.0f, 7.0f);
    QuadTreeInsert(tree, 1.0f, 1.0f);
    // Target outside the tree bounds - search should still find closest point
    Point* nn = QuadTreeNearestNeighbor(tree, 10.0f, 10.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 7.0f);
    EXPECT_FLOAT_EQ(nn->y, 7.0f);
}

TEST(QuadTreeNNTest, PointAcrossBoundary) {
    QuadTree tree(0.0f, 8.0f, 0.0f, 8.0f);
    QuadTreeInsert(tree, 3.9f, 4.0f);  // just left of midline x=4
    QuadTreeInsert(tree, 4.1f, 4.0f);  // just right of midline x=4
    QuadTreeInsert(tree, 0.1f, 0.1f);  // far away
    Point* nn = QuadTreeNearestNeighbor(tree, 4.05f, 4.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 4.1f);
}

TEST(QuadTreeNNTest, NNOnManyPoints) {
    QuadTree tree(0.0f, 10.0f, 0.0f, 10.0f);
    std::vector<std::pair<float,float>> pts = {
        {0.5f,0.5f},{2.3f,1.7f},{4.1f,6.2f},
        {7.8f,3.3f},{5.0f,5.0f},{9.1f,8.5f},
        {1.2f,9.0f},{6.6f,0.9f},{3.3f,7.7f}
    };
    for (auto& [x,y] : pts) QuadTreeInsert(tree, x, y);

    // For target (5.1, 5.1), closest should be (5.0, 5.0)
    Point* nn = QuadTreeNearestNeighbor(tree, 5.1f, 5.1f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 5.0f);
    EXPECT_FLOAT_EQ(nn->y, 5.0f);
}
