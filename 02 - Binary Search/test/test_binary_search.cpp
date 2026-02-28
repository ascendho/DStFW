#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "Binary Search/BinarySearch.hpp"

TEST(BinarySearchTest, FindsElement) {
    std::vector<int> v{1, 2, 5, 5, 6, 9}; // 必须有序
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
    // 二分查找不保证找到哪个重复元素，只保证能找到一个。
    std::vector<int> v{1, 2, 5, 5, 6, 9};
    int index = BinarySearch(v, 5);
    EXPECT_TRUE(index == 2 || index == 3);
}
