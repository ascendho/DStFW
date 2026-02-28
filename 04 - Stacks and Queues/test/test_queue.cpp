#include <gtest/gtest.h>
#include "Queue/QueueArray.hpp"
#include "Queue/QueueList.hpp"

// ─── QueueArray ───────────────────────────────────────────────────────────────

TEST(QueueArrayTest, EnqueueAndDequeue) {
    QueueArray<int> q;
    Enqueue(q, 1); Enqueue(q, 2); Enqueue(q, 3);
    EXPECT_EQ(Dequeue(q), 1);
    EXPECT_EQ(Dequeue(q), 2);
    EXPECT_EQ(Dequeue(q), 3);
}

TEST(QueueArrayTest, DequeueEmptyReturnsNullopt) {
    QueueArray<int> q;
    EXPECT_EQ(Dequeue(q), std::nullopt);
}

TEST(QueueArrayTest, FIFOOrdering) {
    QueueArray<int> q;
    for (int i = 1; i <= 5; ++i) Enqueue(q, i);
    for (int i = 1; i <= 5; ++i) {
        ASSERT_EQ(Dequeue(q), i);
    }
}

TEST(QueueArrayTest, WrapAround) {
    // capacity = 4; dequeue some then enqueue more to trigger wrap-around
    QueueArray<int> q(4);
    Enqueue(q, 1); Enqueue(q, 2); Enqueue(q, 3);
    EXPECT_EQ(Dequeue(q), 1);
    EXPECT_EQ(Dequeue(q), 2);
    // Now front == 2, back == 2; enqueue two more to wrap back around
    Enqueue(q, 4); Enqueue(q, 5);
    EXPECT_EQ(Dequeue(q), 3);
    EXPECT_EQ(Dequeue(q), 4);
    EXPECT_EQ(Dequeue(q), 5);
}

TEST(QueueArrayTest, ArrayDoubling) {
    QueueArray<int> q(4);
    for (int i = 0; i < 20; ++i) Enqueue(q, i);
    for (int i = 0; i < 20; ++i) {
        ASSERT_EQ(Dequeue(q), i);
    }
}

TEST(QueueArrayTest, IsEmpty) {
    QueueArray<int> q;
    EXPECT_TRUE(IsEmpty(q));
    Enqueue(q, 7);
    EXPECT_FALSE(IsEmpty(q));
    Dequeue(q);
    EXPECT_TRUE(IsEmpty(q));
}

TEST(QueueArrayTest, FrontPeek) {
    QueueArray<int> q;
    EXPECT_EQ(Front(q), std::nullopt);
    Enqueue(q, 10);
    EXPECT_EQ(Front(q), 10);
    EXPECT_EQ(Front(q), 10);  // still in queue
}

// ─── QueueList ────────────────────────────────────────────────────────────────

TEST(QueueListTest, EnqueueAndDequeue) {
    QueueList<int> q;
    Enqueue(q, 1); Enqueue(q, 2); Enqueue(q, 3);
    EXPECT_EQ(Dequeue(q), 1);
    EXPECT_EQ(Dequeue(q), 2);
    EXPECT_EQ(Dequeue(q), 3);
}

TEST(QueueListTest, DequeueEmptyReturnsNullopt) {
    QueueList<int> q;
    EXPECT_EQ(Dequeue(q), std::nullopt);
}

TEST(QueueListTest, FIFOOrdering) {
    QueueList<int> q;
    for (int i = 1; i <= 5; ++i) Enqueue(q, i);
    for (int i = 1; i <= 5; ++i) {
        ASSERT_EQ(Dequeue(q), i);
    }
}

TEST(QueueListTest, IsEmpty) {
    QueueList<int> q;
    EXPECT_TRUE(IsEmpty(q));
    Enqueue(q, 42);
    EXPECT_FALSE(IsEmpty(q));
    Dequeue(q);
    EXPECT_TRUE(IsEmpty(q));
}

TEST(QueueListTest, SingleElementEnqueueDequeue) {
    QueueList<int> q;
    Enqueue(q, 99);
    EXPECT_EQ(Dequeue(q), 99);
    // After removing the last element, both front and back should be null
    EXPECT_TRUE(IsEmpty(q));
    // A subsequent enqueue should still work correctly
    Enqueue(q, 5);
    EXPECT_EQ(Dequeue(q), 5);
}

TEST(QueueListTest, FrontPeek) {
    QueueList<int> q;
    EXPECT_EQ(Front(q), std::nullopt);
    Enqueue(q, 3);
    EXPECT_EQ(Front(q), 3);
    EXPECT_EQ(Front(q), 3);  // still in queue
}

TEST(QueueListTest, LargeNumberOfElements) {
    QueueList<int> q;
    for (int i = 0; i < 1000; ++i) Enqueue(q, i);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(Dequeue(q), i);
    }
    EXPECT_TRUE(IsEmpty(q));
}
