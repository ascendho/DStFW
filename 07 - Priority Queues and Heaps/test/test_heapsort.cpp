#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "Heapsort/Heapsort.hpp"

// ─── Heapsort (decreasing) ────────────────────────────────────────────────────

TEST(HeapsortTest, SortsDecreasing) {
    std::vector<int> v{46, 35, 9, 28, 61, 8, 38, 40};
    auto result = Heapsort(v);
    std::vector<int> expected = v;
    std::sort(expected.rbegin(), expected.rend());
    EXPECT_EQ(result, expected);
}

TEST(HeapsortTest, AlreadySortedDecreasing) {
    std::vector<int> v{9, 7, 5, 3, 1};
    auto result = Heapsort(v);
    EXPECT_EQ(result, v);
}

TEST(HeapsortTest, AlreadySortedIncreasing) {
    std::vector<int> v{1, 3, 5, 7, 9};
    auto result = Heapsort(v);
    std::vector<int> expected{9, 7, 5, 3, 1};
    EXPECT_EQ(result, expected);
}

TEST(HeapsortTest, SingleElement) {
    std::vector<int> v{42};
    EXPECT_EQ(Heapsort(v), v);
}

TEST(HeapsortTest, EmptyVector) {
    std::vector<int> v{};
    EXPECT_EQ(Heapsort(v), v);
}

TEST(HeapsortTest, DuplicateValues) {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto result = Heapsort(v);
    std::vector<int> expected = v;
    std::sort(expected.rbegin(), expected.rend());
    EXPECT_EQ(result, expected);
}

// ─── HeapsortAsc (increasing, min heap) ──────────────────────────────────────

TEST(HeapsortAscTest, SortsIncreasing) {
    std::vector<int> v{46, 35, 9, 28, 61, 8, 38, 40};
    auto result = HeapsortAsc(v);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST(HeapsortAscTest, DuplicateValues) {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    auto result = HeapsortAsc(v);
    std::vector<int> expected = v;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

TEST(HeapsortAscTest, EmptyVector) {
    std::vector<int> v{};
    EXPECT_EQ(HeapsortAsc(v), v);
}
