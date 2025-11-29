#include <gtest/gtest.h>
#include <vector>
#include "Linear Scan/LinearScan.hpp"

TEST(LinearScanTest, FindsElement) {
    std::vector<int> v{5, 2, 9, 1, 5, 6};
    int index = LinearScan(v, 9);
    EXPECT_EQ(index, 2);
}

TEST(LinearScanTest, ElementNotFound) {
    std::vector<int> v{5, 2, 9, 1, 5, 6};
    int index = LinearScan(v, 10);
    EXPECT_EQ(index, -1);
}

TEST(LinearScanTest, HandlesEmpty) {
    std::vector<int> v{};
    int index = LinearScan(v, 5);
    EXPECT_EQ(index, -1);
}

TEST(LinearScanTest, FindsFirstOccurrence) {
    std::vector<int> v{5, 2, 9, 1, 5, 6};
    int index = LinearScan(v, 5);
    EXPECT_EQ(index, 0);
}
