#pragma once

// DoublyLinkedListNode - Chapter 3: Dynamic Data Structures
// A doubly linked list node with both forward and backward pointers.
//
// Pseudocode:
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
