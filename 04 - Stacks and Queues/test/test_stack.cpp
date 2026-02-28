#include <gtest/gtest.h>
#include "Stack/StackArray.hpp"
#include "Stack/StackList.hpp"

// ─── StackArray ───────────────────────────────────────────────────────────────

TEST(StackArrayTest, PushAndPop) {
    StackArray<int> s;
    Push(s, 1); Push(s, 2); Push(s, 3);
    EXPECT_EQ(Pop(s), 3);
    EXPECT_EQ(Pop(s), 2);
    EXPECT_EQ(Pop(s), 1);
}

TEST(StackArrayTest, PopEmptyReturnsNullopt) {
    StackArray<int> s;
    EXPECT_EQ(Pop(s), std::nullopt);
}

TEST(StackArrayTest, LIFOOrdering) {
    StackArray<int> s;
    for (int i = 1; i <= 5; ++i) Push(s, i);
    for (int i = 5; i >= 1; --i) {
        ASSERT_EQ(Pop(s), i);
    }
}

TEST(StackArrayTest, ArrayDoubling) {
    // initial capacity = 4; push more to trigger doubling
    StackArray<int> s(4);
    for (int i = 0; i < 20; ++i) Push(s, i);
    for (int i = 19; i >= 0; --i) {
        ASSERT_EQ(Pop(s), i);
    }
}

TEST(StackArrayTest, IsEmpty) {
    StackArray<int> s;
    EXPECT_TRUE(IsEmpty(s));
    Push(s, 42);
    EXPECT_FALSE(IsEmpty(s));
    Pop(s);
    EXPECT_TRUE(IsEmpty(s));
}

TEST(StackArrayTest, TopPeek) {
    StackArray<int> s;
    EXPECT_EQ(Top(s), std::nullopt);
    Push(s, 7);
    EXPECT_EQ(Top(s), 7);
    // Top does not remove the element
    EXPECT_EQ(Top(s), 7);
}

// ─── StackList ────────────────────────────────────────────────────────────────

TEST(StackListTest, PushAndPop) {
    StackList<int> s;
    Push(s, 1); Push(s, 2); Push(s, 3);
    EXPECT_EQ(Pop(s), 3);
    EXPECT_EQ(Pop(s), 2);
    EXPECT_EQ(Pop(s), 1);
}

TEST(StackListTest, PopEmptyReturnsNullopt) {
    StackList<int> s;
    EXPECT_EQ(Pop(s), std::nullopt);
}

TEST(StackListTest, LIFOOrdering) {
    StackList<int> s;
    for (int i = 1; i <= 5; ++i) Push(s, i);
    for (int i = 5; i >= 1; --i) {
        ASSERT_EQ(Pop(s), i);
    }
}

TEST(StackListTest, IsEmpty) {
    StackList<int> s;
    EXPECT_TRUE(IsEmpty(s));
    Push(s, 99);
    EXPECT_FALSE(IsEmpty(s));
    Pop(s);
    EXPECT_TRUE(IsEmpty(s));
}

TEST(StackListTest, TopPeek) {
    StackList<int> s;
    EXPECT_EQ(Top(s), std::nullopt);
    Push(s, 5);
    EXPECT_EQ(Top(s), 5);
    EXPECT_EQ(Top(s), 5);  // still there
}

TEST(StackListTest, LargeNumberOfElements) {
    StackList<int> s;
    for (int i = 0; i < 1000; ++i) Push(s, i);
    for (int i = 999; i >= 0; --i) {
        ASSERT_EQ(Pop(s), i);
    }
    EXPECT_TRUE(IsEmpty(s));
}
