#include <gtest/gtest.h>
#include <vector>
#include "Array Doubling/ArrayDoubling.hpp"

TEST(ArrayDoubleTest, DoublesSize) {
    std::vector<int> v{1, 2, 3, 4};
    auto result = ArrayDouble(v);
    EXPECT_EQ(result.size(), 8u);
}

TEST(ArrayDoubleTest, CopiesElements) {
    std::vector<int> v{3, 11, 9, 37, 7, 8};
    auto result = ArrayDouble(v);
    for (int i = 0; i < static_cast<int>(v.size()); ++i) {
        EXPECT_EQ(result[i], v[i]);
    }
}

TEST(ArrayDoubleTest, EmptyArray) {
    std::vector<int> v{};
    auto result = ArrayDouble(v);
    EXPECT_EQ(result.size(), 0u);
}

TEST(ArrayDoubleTest, SingleElement) {
    std::vector<double> v{3.14};
    auto result = ArrayDouble(v);
    EXPECT_EQ(result.size(), 2u);
    EXPECT_DOUBLE_EQ(result[0], 3.14);
}

TEST(ArrayDoubleTest, NewElementsDefaultInitialized) {
    std::vector<int> v{1, 2};
    auto result = ArrayDouble(v);
    // 后半部分应为默认初始化（int 类型为 0）
    EXPECT_EQ(result[2], 0);
    EXPECT_EQ(result[3], 0);
}
