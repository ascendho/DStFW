#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "Binary Search/BinarySearch.hpp"

TEST(BinarySearchTest, FindsElement) {
    std::vector<int> v{1, 2, 5, 5, 6, 9}; // Must be sorted
    int index = BinarySearch(v, 9);
    EXPECT_EQ(index, 5);
}

TEST(BinarySearchTest, ElementNotFound) {
    std::vector<int> v{1, 2, 5, 5, 6, 9};
    int index = BinarySearch(v, 10);
    EXPECT_EQ(index, -1);
}

TEST(BinarySearchTest, HandlesEmpty) {
    std::vector<int> v{};
    int index = BinarySearch(v, 5);
    EXPECT_EQ(index, -1);
}

TEST(BinarySearchTest, FindsOneOfDuplicates) {
    // Binary search doesn't guarantee *which* duplicate it finds, just that it finds one.
    std::vector<int> v{1, 2, 5, 5, 6, 9};
    int index = BinarySearch(v, 5);
    EXPECT_TRUE(index == 2 || index == 3);
}
