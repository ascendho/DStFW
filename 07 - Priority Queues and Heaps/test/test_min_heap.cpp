#include <gtest/gtest.h>
#include <vector>
#include "Min Heap/MinHeap.hpp"

// ─── MinHeapInsert / MinHeapRemoveMin ─────────────────────────────────────────

TEST(MinHeapTest, InsertSingleAndRemove) {
    Heap<int> h;
    MinHeapInsert(h, 7);
    EXPECT_EQ(MinHeapRemoveMin(h), 7);
    EXPECT_TRUE(IsMinEmpty(h));
}

TEST(MinHeapTest, RemoveFromEmptyReturnsNullopt) {
    Heap<int> h;
    EXPECT_EQ(MinHeapRemoveMin(h), std::nullopt);
}

TEST(MinHeapTest, MinAlwaysAtRoot) {
    Heap<int> h;
    for (int v : {3, 11, 9, 37, 7, 8}) MinHeapInsert(h, v);
    EXPECT_EQ(HeapMin(h), 3);
}

TEST(MinHeapTest, RemovesInIncreasingOrder) {
    Heap<int> h;
    for (int v : {5, 1, 3, 2, 4}) MinHeapInsert(h, v);
    for (int expected : {1, 2, 3, 4, 5}) {
        ASSERT_EQ(MinHeapRemoveMin(h), expected);
    }
    EXPECT_TRUE(IsMinEmpty(h));
}

TEST(MinHeapTest, HeapProperty_ParentLeChild) {
    Heap<int> h;
    for (int v : {46, 35, 9, 28, 61, 8, 38, 40}) MinHeapInsert(h, v);
    for (int i = 2; i <= h.last_index; ++i) {
        int parent = i / 2;
        EXPECT_LE(h.array[parent], h.array[i])
            << "Min-heap property violated at index " << i;
    }
}

TEST(MinHeapTest, ArrayDoublingOnGrowth) {
    Heap<int> h(4);
    for (int i = 30; i >= 1; --i) MinHeapInsert(h, i);
    for (int i = 1; i <= 30; ++i) {
        ASSERT_EQ(MinHeapRemoveMin(h), i);
    }
}

TEST(MinHeapTest, DuplicateValues) {
    Heap<int> h;
    for (int v : {2, 2, 2, 5, 5}) MinHeapInsert(h, v);
    EXPECT_EQ(MinHeapRemoveMin(h), 2);
    EXPECT_EQ(MinHeapRemoveMin(h), 2);
    EXPECT_EQ(MinHeapRemoveMin(h), 2);
    EXPECT_EQ(MinHeapRemoveMin(h), 5);
    EXPECT_EQ(MinHeapRemoveMin(h), 5);
}
