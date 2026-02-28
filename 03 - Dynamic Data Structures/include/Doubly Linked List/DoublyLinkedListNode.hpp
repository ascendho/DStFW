#pragma once

// DoublyLinkedListNode — 第三章：动态数据结构
// 双链表节点，包含前向和后向指针。
//
// 伪代码：
//   DoublyLinkedListNode {
//       Type: Value
//       DoublyLinkedListNode: next
//       DoublyLinkedListNode: previous
//   }
template<typename T>
struct DoublyLinkedListNode {
    T value;
    DoublyLinkedListNode<T>* next;
    DoublyLinkedListNode<T>* previous;

    explicit DoublyLinkedListNode(const T& val)
        : value(val), next(nullptr), previous(nullptr) {}
};
