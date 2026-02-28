#include <gtest/gtest.h>
#include <cmath>
#include "Grid/Grid.hpp"
#include "NearestNeighbor/NearestNeighbor.hpp"

// ─── Helper ───────────────────────────────────────────────────────────────────
static bool grid_contains(const Grid& g, float x, float y) {
    int xbin = static_cast<int>(std::floor((x - g.x_start) / g.x_bin_width));
    int ybin = static_cast<int>(std::floor((y - g.y_start) / g.y_bin_width));
    if (xbin < 0 || xbin >= g.num_x_bins) return false;
    if (ybin < 0 || ybin >= g.num_y_bins) return false;
    GridPoint* cur = g.bins[xbin][ybin];
    while (cur) {
        if (approx_equal(cur->x, cur->y, x, y)) return true;
        cur = cur->next;
    }
    return false;
}

// ─── Grid Construction ────────────────────────────────────────────────────────
TEST(GridTest, ConstructionDimensions) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_EQ(g.num_x_bins, 4);
    EXPECT_EQ(g.num_y_bins, 4);
    EXPECT_FLOAT_EQ(g.x_bin_width, 2.0f);
    EXPECT_FLOAT_EQ(g.y_bin_width, 2.0f);
    EXPECT_FLOAT_EQ(g.x_start, 0.0f);
    EXPECT_FLOAT_EQ(g.x_end,   8.0f);
    EXPECT_FLOAT_EQ(g.y_start, 0.0f);
    EXPECT_FLOAT_EQ(g.y_end,   8.0f);
}

TEST(GridTest, AllBinsInitiallyEmpty) {
    Grid g(3, 3, 0.0f, 3.0f, 0.0f, 3.0f);
    for (int xi = 0; xi < 3; ++xi)
        for (int yi = 0; yi < 3; ++yi)
            EXPECT_EQ(g.bins[xi][yi], nullptr);
}

// ─── GridInsert ───────────────────────────────────────────────────────────────
TEST(GridInsertTest, InsertSinglePoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_TRUE(GridInsert(g, 1.0f, 1.0f));
    EXPECT_TRUE(grid_contains(g, 1.0f, 1.0f));
}

TEST(GridInsertTest, InsertMultiplePointsSameBin) {
    Grid g(2, 2, 0.0f, 4.0f, 0.0f, 4.0f);
    // All in bin (0,0)
    EXPECT_TRUE(GridInsert(g, 0.5f, 0.5f));
    EXPECT_TRUE(GridInsert(g, 1.0f, 1.0f));
    EXPECT_TRUE(GridInsert(g, 1.5f, 1.5f));
    EXPECT_EQ(g.bins[0][0]->x, 1.5f); // most recently inserted is at front
    EXPECT_TRUE(grid_contains(g, 0.5f, 0.5f));
    EXPECT_TRUE(grid_contains(g, 1.0f, 1.0f));
    EXPECT_TRUE(grid_contains(g, 1.5f, 1.5f));
}

TEST(GridInsertTest, InsertPointOutsideBounds) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_FALSE(GridInsert(g, -1.0f,  4.0f));
    EXPECT_FALSE(GridInsert(g,  4.0f, -1.0f));
    EXPECT_FALSE(GridInsert(g,  9.0f,  4.0f));
    EXPECT_FALSE(GridInsert(g,  4.0f,  9.0f));
}

TEST(GridInsertTest, InsertAtBinBoundary) {
    Grid g(4, 4, 0.0f, 4.0f, 0.0f, 4.0f);
    // x=1.0 is the boundary between bin 0 and bin 1
    EXPECT_TRUE(GridInsert(g, 1.0f, 1.0f));
    // Exactly at x_end (8.0) is outside the grid
    EXPECT_FALSE(GridInsert(g, 4.0f, 2.0f));
}

TEST(GridInsertTest, InsertDuplicatePoints) {
    Grid g(2, 2, 0.0f, 4.0f, 0.0f, 4.0f);
    EXPECT_TRUE(GridInsert(g, 1.0f, 1.0f));
    EXPECT_TRUE(GridInsert(g, 1.0f, 1.0f)); // duplicates are allowed
    // Both should be present (two nodes in the list)
    int count = 0;
    GridPoint* cur = g.bins[0][0];
    while (cur) { ++count; cur = cur->next; }
    EXPECT_EQ(count, 2);
}

// ─── GridDelete ───────────────────────────────────────────────────────────────
TEST(GridDeleteTest, DeleteExistingPoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 1.0f, 1.0f);
    EXPECT_TRUE(GridDelete(g, 1.0f, 1.0f));
    EXPECT_FALSE(grid_contains(g, 1.0f, 1.0f));
}

TEST(GridDeleteTest, DeleteNonExistentPoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_FALSE(GridDelete(g, 5.0f, 5.0f));
}

TEST(GridDeleteTest, DeleteOutOfBoundsPoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_FALSE(GridDelete(g, -1.0f, 0.0f));
    EXPECT_FALSE(GridDelete(g, 9.0f, 0.0f));
}

TEST(GridDeleteTest, DeleteFirstOfMultiple) {
    Grid g(2, 2, 0.0f, 4.0f, 0.0f, 4.0f);
    GridInsert(g, 0.5f, 0.5f);
    GridInsert(g, 1.0f, 1.0f);
    GridInsert(g, 1.5f, 1.5f);
    // Delete the head node (most recently inserted: 1.5,1.5)
    EXPECT_TRUE(GridDelete(g, 1.5f, 1.5f));
    EXPECT_FALSE(grid_contains(g, 1.5f, 1.5f));
    EXPECT_TRUE(grid_contains(g, 1.0f, 1.0f));
    EXPECT_TRUE(grid_contains(g, 0.5f, 0.5f));
}

TEST(GridDeleteTest, DeleteMiddleOfList) {
    Grid g(2, 2, 0.0f, 4.0f, 0.0f, 4.0f);
    GridInsert(g, 0.5f, 0.5f);
    GridInsert(g, 1.0f, 1.0f);
    GridInsert(g, 1.5f, 1.5f);
    EXPECT_TRUE(GridDelete(g, 1.0f, 1.0f));
    EXPECT_FALSE(grid_contains(g, 1.0f, 1.0f));
    EXPECT_TRUE(grid_contains(g, 1.5f, 1.5f));
    EXPECT_TRUE(grid_contains(g, 0.5f, 0.5f));
}

TEST(GridDeleteTest, DeleteOnlyRemovesFirstDuplicate) {
    Grid g(2, 2, 0.0f, 4.0f, 0.0f, 4.0f);
    GridInsert(g, 1.0f, 1.0f);
    GridInsert(g, 1.0f, 1.0f);
    EXPECT_TRUE(GridDelete(g, 1.0f, 1.0f));
    // One copy should remain
    EXPECT_TRUE(grid_contains(g, 1.0f, 1.0f));
    EXPECT_TRUE(GridDelete(g, 1.0f, 1.0f));
    EXPECT_FALSE(grid_contains(g, 1.0f, 1.0f));
}

// ─── MinDistToBin ─────────────────────────────────────────────────────────────
TEST(MinDistToBinTest, PointInsideBin) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    // Bin (1,1) covers x:[2,4], y:[2,4].  Point (3,3) is inside → dist = 0
    EXPECT_FLOAT_EQ(MinDistToBin(g, 1, 1, 3.0f, 3.0f), 0.0f);
}

TEST(MinDistToBinTest, PointOutsideBin) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    // Bin (0,0) covers x:[0,2], y:[0,2].  Point (3,1) → x_dist=1, y_dist=0
    EXPECT_FLOAT_EQ(MinDistToBin(g, 0, 0, 3.0f, 1.0f), 1.0f);
}

TEST(MinDistToBinTest, InvalidBin) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    float inf = std::numeric_limits<float>::infinity();
    EXPECT_EQ(MinDistToBin(g, -1, 0, 0.0f, 0.0f), inf);
    EXPECT_EQ(MinDistToBin(g, 0, -1, 0.0f, 0.0f), inf);
    EXPECT_EQ(MinDistToBin(g, 4,  0, 0.0f, 0.0f), inf);
}

// ─── GridLinearScanNN ─────────────────────────────────────────────────────────
TEST(GridLinearScanNNTest, EmptyGridReturnsNull) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_EQ(GridLinearScanNN(g, 4.0f, 4.0f), nullptr);
}

TEST(GridLinearScanNNTest, SinglePoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 3.0f, 3.0f);
    GridPoint* nn = GridLinearScanNN(g, 4.0f, 4.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 3.0f);
    EXPECT_FLOAT_EQ(nn->y, 3.0f);
}

TEST(GridLinearScanNNTest, MultiplePoints) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 1.0f, 1.0f);
    GridInsert(g, 6.0f, 6.0f);
    GridInsert(g, 4.0f, 4.0f);
    // Target (4.1, 4.1) — closest is (4.0, 4.0)
    GridPoint* nn = GridLinearScanNN(g, 4.1f, 4.1f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 4.0f);
    EXPECT_FLOAT_EQ(nn->y, 4.0f);
}

TEST(GridLinearScanNNTest, TargetOutsideGrid) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 7.0f, 7.0f);
    GridInsert(g, 1.0f, 1.0f);
    // Target (10.0, 10.0) is outside; closest should be (7.0, 7.0)
    GridPoint* nn = GridLinearScanNN(g, 10.0f, 10.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 7.0f);
    EXPECT_FLOAT_EQ(nn->y, 7.0f);
}

TEST(GridLinearScanNNTest, TieBreakFirstFound) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    // Two equidistant points
    GridInsert(g, 2.0f, 4.0f);
    GridInsert(g, 6.0f, 4.0f);
    GridPoint* nn = GridLinearScanNN(g, 4.0f, 4.0f);
    ASSERT_NE(nn, nullptr);
    // Both are at distance 2; the function returns one of them
    float d = euclidean_dist(4.0f, 4.0f, nn->x, nn->y);
    EXPECT_FLOAT_EQ(d, 2.0f);
}

// ─── GridSearchExpanding ──────────────────────────────────────────────────────
TEST(GridSearchExpandingTest, EmptyGridReturnsNull) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    EXPECT_EQ(GridSearchExpanding(g, 4.0f, 4.0f), nullptr);
}

TEST(GridSearchExpandingTest, SinglePoint) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 3.0f, 3.0f);
    GridPoint* nn = GridSearchExpanding(g, 4.0f, 4.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 3.0f);
    EXPECT_FLOAT_EQ(nn->y, 3.0f);
}

TEST(GridSearchExpandingTest, MultiplePointsMatchLinearScan) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 1.0f, 1.0f);
    GridInsert(g, 6.0f, 6.0f);
    GridInsert(g, 4.0f, 4.0f);
    GridInsert(g, 5.0f, 2.0f);

    float tx = 4.2f, ty = 3.8f;
    GridPoint* ls = GridLinearScanNN(g, tx, ty);
    GridPoint* es = GridSearchExpanding(g, tx, ty);
    ASSERT_NE(ls, nullptr);
    ASSERT_NE(es, nullptr);
    // Both should find the same nearest-neighbour point
    EXPECT_FLOAT_EQ(euclidean_dist(tx, ty, ls->x, ls->y),
                    euclidean_dist(tx, ty, es->x, es->y));
}

TEST(GridSearchExpandingTest, TargetOutsideGrid) {
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 7.0f, 7.0f);
    GridInsert(g, 1.0f, 1.0f);
    GridPoint* nn = GridSearchExpanding(g, 10.0f, 10.0f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 7.0f);
    EXPECT_FLOAT_EQ(nn->y, 7.0f);
}

TEST(GridSearchExpandingTest, PointInAdjacentBin) {
    // Target is in one bin but the nearest point is in the adjacent bin
    Grid g(4, 4, 0.0f, 8.0f, 0.0f, 8.0f);
    GridInsert(g, 2.1f, 2.1f); // bin (1,1) — just across the border
    GridInsert(g, 6.0f, 6.0f);
    // Target near (2.0, 2.0) — the bin boundary
    GridPoint* nn = GridSearchExpanding(g, 1.9f, 1.9f);
    ASSERT_NE(nn, nullptr);
    EXPECT_FLOAT_EQ(nn->x, 2.1f);
    EXPECT_FLOAT_EQ(nn->y, 2.1f);
}

TEST(GridSearchExpandingTest, ConsistentWithLinearScanOnRandomPoints) {
    Grid g(5, 5, 0.0f, 10.0f, 0.0f, 10.0f);
    // Insert a spread of points
    float pts[][2] = {
        {0.5f, 0.5f}, {2.3f, 1.7f}, {4.1f, 6.2f},
        {7.8f, 3.3f}, {5.0f, 5.0f}, {9.1f, 8.5f},
        {1.2f, 9.0f}, {6.6f, 0.9f}, {3.3f, 7.7f}
    };
    for (auto& p : pts) GridInsert(g, p[0], p[1]);

    float targets[][2] = {
        {0.0f, 0.0f}, {5.0f, 5.0f}, {9.9f, 9.9f}, {3.0f, 4.0f}
    };
    for (auto& t : targets) {
        GridPoint* ls = GridLinearScanNN(g, t[0], t[1]);
        GridPoint* es = GridSearchExpanding(g, t[0], t[1]);
        ASSERT_NE(ls, nullptr);
        ASSERT_NE(es, nullptr);
        EXPECT_FLOAT_EQ(euclidean_dist(t[0], t[1], ls->x, ls->y),
                        euclidean_dist(t[0], t[1], es->x, es->y))
            << "Mismatch for target (" << t[0] << ", " << t[1] << "): "
            << "LS=(" << ls->x << "," << ls->y << ") "
            << "ES=(" << es->x << "," << es->y << ")";
    }
}
