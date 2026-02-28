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
    // 容量 = 4；先出队一些再入队以触发环绕
    QueueArray<int> q(4);
    Enqueue(q, 1); Enqueue(q, 2); Enqueue(q, 3);
    EXPECT_EQ(Dequeue(q), 1);
    EXPECT_EQ(Dequeue(q), 2);
    // 此时 front == 2, back == 2；再入队两个以触发回绕
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
    EXPECT_EQ(Front(q), 10);  // 仍在队列中
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
    // 移除最后一个元素后，front 和 back 都应为 null
    EXPECT_TRUE(IsEmpty(q));
    // 后续入队操作仍应正常工作
    Enqueue(q, 5);
    EXPECT_EQ(Dequeue(q), 5);
}

TEST(QueueListTest, FrontPeek) {
    QueueList<int> q;
    EXPECT_EQ(Front(q), std::nullopt);
    Enqueue(q, 3);
    EXPECT_EQ(Front(q), 3);
    EXPECT_EQ(Front(q), 3);  // 仍在队列中
}

TEST(QueueListTest, LargeNumberOfElements) {
    QueueList<int> q;
    for (int i = 0; i < 1000; ++i) Enqueue(q, i);
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(Dequeue(q), i);
    }
    EXPECT_TRUE(IsEmpty(q));
}
