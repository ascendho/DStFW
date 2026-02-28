#include <gtest/gtest.h>
#include <vector>
#include "Linked List/LinkedList.hpp"

// Helper: build a linked list from a vector (caller takes ownership of head)
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

// Helper: collect list values into a vector and free the list
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
    // Insert 23 between index 2 (value 9) and index 3 (value 37)
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
    // Delete node with value 37 (index 3) from {3, 11, 9, 37, 7, 8}
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
