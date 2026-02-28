#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "Max Heap/MaxHeap.hpp"

// ─── HeapInsert / HeapRemoveMax ───────────────────────────────────────────────

TEST(MaxHeapTest, InsertSingleAndRemove) {
    Heap<int> h;
    HeapInsert(h, 42);
    EXPECT_EQ(HeapRemoveMax(h), 42);
    EXPECT_TRUE(IsEmpty(h));
}

TEST(MaxHeapTest, RemoveFromEmptyReturnsNullopt) {
    Heap<int> h;
    EXPECT_EQ(HeapRemoveMax(h), std::nullopt);
}

TEST(MaxHeapTest, MaxAlwaysAtRoot) {
    Heap<int> h;
    for (int v : {3, 11, 9, 37, 7, 8}) HeapInsert(h, v);
    EXPECT_EQ(HeapMax(h), 37);
}

TEST(MaxHeapTest, RemovesInDecreasingOrder) {
    Heap<int> h;
    for (int v : {5, 1, 3, 2, 4}) HeapInsert(h, v);
    for (int expected : {5, 4, 3, 2, 1}) {
        ASSERT_EQ(HeapRemoveMax(h), expected);
    }
    EXPECT_TRUE(IsEmpty(h));
}

TEST(MaxHeapTest, ArrayDoublingOnGrowth) {
    // Start with small capacity (4), insert many elements
    Heap<int> h(4);
    for (int i = 1; i <= 30; ++i) HeapInsert(h, i);
    // Should still extract correctly
    for (int i = 30; i >= 1; --i) {
        ASSERT_EQ(HeapRemoveMax(h), i);
    }
}

TEST(MaxHeapTest, DuplicateValues) {
    Heap<int> h;
    for (int v : {5, 5, 5, 3, 3}) HeapInsert(h, v);
    EXPECT_EQ(HeapRemoveMax(h), 5);
    EXPECT_EQ(HeapRemoveMax(h), 5);
    EXPECT_EQ(HeapRemoveMax(h), 5);
    EXPECT_EQ(HeapRemoveMax(h), 3);
    EXPECT_EQ(HeapRemoveMax(h), 3);
}

TEST(MaxHeapTest, HeapProperty_ParentGeChild) {
    // After inserting unsorted data, verify heap property at every node
    Heap<int> h;
    for (int v : {46, 35, 9, 28, 61, 8, 38, 40}) HeapInsert(h, v);
    for (int i = 2; i <= h.last_index; ++i) {
        int parent = i / 2;
        EXPECT_GE(h.array[parent], h.array[i])
            << "Heap property violated at index " << i;
    }
}

// ─── UpdateMaxValue ───────────────────────────────────────────────────────────

TEST(MaxHeapTest, UpdateIncreaseValue) {
    Heap<int> h;
    for (int v : {10, 5, 8, 3, 2}) HeapInsert(h, v);
    // Find index of value 3 (it's in the heap; let's find it)
    int idx = -1;
    for (int i = 1; i <= h.last_index; ++i)
        if (h.array[i] == 3) { idx = i; break; }
    ASSERT_GT(idx, 0);
    UpdateMaxValue(h, idx, 20);  // raise 3 → 20 (should become new max)
    EXPECT_EQ(HeapMax(h), 20);
    // Heap property must still hold
    for (int i = 2; i <= h.last_index; ++i)
        EXPECT_GE(h.array[i / 2], h.array[i]);
}

TEST(MaxHeapTest, UpdateDecreaseValue) {
    Heap<int> h;
    for (int v : {10, 5, 8, 3, 2}) HeapInsert(h, v);
    // Decrease root (index 1) to 1
    UpdateMaxValue(h, 1, 1);
    // Heap property must still hold
    for (int i = 2; i <= h.last_index; ++i)
        EXPECT_GE(h.array[i / 2], h.array[i]);
}
