#include <gtest/gtest.h>
#include <vector>
#include "Linked List/LinkedList.hpp"

// 辅助函数：从 vector 构建链表（调用者负责管理 head 的所有权）
template<typename T>
LinkedListNode<T>* makeList(const std::vector<T>& values) {
    if (values.empty()) return nullptr;
    LinkedListNode<T>* head = new LinkedListNode<T>(values[0]);
    LinkedListNode<T>* current = head;
    for (std::size_t i = 1; i < values.size(); ++i) {
        current->next = new LinkedListNode<T>(values[i]);
        current = current->next;
    }
    return head;
}

// 辅助函数：将链表值收集到 vector 中并释放链表
template<typename T>
std::vector<T> collectAndFree(LinkedListNode<T>* head) {
    std::vector<T> result;
    while (head != nullptr) {
        result.push_back(head->value);
        LinkedListNode<T>* next = head->next;
        delete head;
        head = next;
    }
    return result;
}

// ─── LinkedListLookUp ─────────────────────────────────────────────────────────

TEST(LinkedListLookUpTest, ReturnsHead) {
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    EXPECT_EQ(LinkedListLookUp(head, 0)->value, 3);
    collectAndFree(head);
}

TEST(LinkedListLookUpTest, ReturnsMiddleNode) {
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    EXPECT_EQ(LinkedListLookUp(head, 3)->value, 37);
    collectAndFree(head);
}

TEST(LinkedListLookUpTest, ReturnsTailNode) {
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    EXPECT_EQ(LinkedListLookUp(head, 5)->value, 8);
    collectAndFree(head);
}

TEST(LinkedListLookUpTest, ReturnsNullForOutOfRange) {
    auto* head = makeList<int>({3, 11, 9});
    EXPECT_EQ(LinkedListLookUp(head, 10), nullptr);
    collectAndFree(head);
}

TEST(LinkedListLookUpTest, ReturnsNullForEmptyList) {
    EXPECT_EQ(LinkedListLookUp<int>(nullptr, 0), nullptr);
}

// ─── LinkedListInsert ─────────────────────────────────────────────────────────

TEST(LinkedListInsertTest, InsertAtFront) {
    auto* head = makeList<int>({3, 11, 9});
    head = LinkedListInsert(head, 0, 23);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{23, 3, 11, 9}));
}

TEST(LinkedListInsertTest, InsertAtEnd) {
    auto* head = makeList<int>({3, 11, 9});
    head = LinkedListInsert(head, 3, 99);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{3, 11, 9, 99}));
}

TEST(LinkedListInsertTest, InsertInMiddle) {
    // 在索引 2（值为 9）和索引 3（值为 37）之间插入 23
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    head = LinkedListInsert(head, 3, 23);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{3, 11, 9, 23, 37, 7, 8}));
}

TEST(LinkedListInsertTest, InsertIntoEmptyList) {
    LinkedListNode<int>* head = nullptr;
    head = LinkedListInsert(head, 0, 42);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{42}));
}

TEST(LinkedListInsertTest, ThrowsOnInvalidIndex) {
    auto* head = makeList<int>({1, 2, 3});
    EXPECT_THROW(LinkedListInsert(head, 5, 99), std::out_of_range);
    collectAndFree(head);
}

// ─── LinkedListDelete ─────────────────────────────────────────────────────────

TEST(LinkedListDeleteTest, DeleteHead) {
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    head = LinkedListDelete(head, 0);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{11, 9, 37, 7, 8}));
}

TEST(LinkedListDeleteTest, DeleteTail) {
    auto* head = makeList<int>({3, 11, 9});
    head = LinkedListDelete(head, 2);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{3, 11}));
}

TEST(LinkedListDeleteTest, DeleteMiddle) {
    // 从 {3, 11, 9, 37, 7, 8} 中删除值为 37（索引 3）的节点
    auto* head = makeList<int>({3, 11, 9, 37, 7, 8});
    head = LinkedListDelete(head, 3);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{3, 11, 9, 7, 8}));
}

TEST(LinkedListDeleteTest, DeleteFromEmptyListReturnsNull) {
    EXPECT_EQ(LinkedListDelete<int>(nullptr, 0), nullptr);
}

TEST(LinkedListDeleteTest, DeleteSingleElement) {
    auto* head = makeList<int>({42});
    head = LinkedListDelete(head, 0);
    EXPECT_EQ(head, nullptr);
}

TEST(LinkedListDeleteTest, ThrowsOnInvalidIndex) {
    auto* head = makeList<int>({1, 2, 3});
    EXPECT_THROW(LinkedListDelete(head, 10), std::out_of_range);
    collectAndFree(head);
}

// ─── LinkedListInsertAfter ────────────────────────────────────────────────────

TEST(LinkedListInsertAfterTest, InsertAfterFirst) {
    auto* head = makeList<int>({1, 3});
    auto* new_node = new LinkedListNode<int>(2);
    LinkedListInsertAfter(head, new_node);
    auto result = collectAndFree(head);
    EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}
