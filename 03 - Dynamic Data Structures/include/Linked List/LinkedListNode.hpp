#pragma once

// LinkedListNode - Chapter 3: Dynamic Data Structures
// A singly linked list node containing a value and a pointer to the next node.
//
// Pseudocode:
//   LinkedListNode {
//       Type: value
//       LinkedListNode: next
//   }
template<typename T>
struct LinkedListNode {
    T value;
    LinkedListNode<T>* next;

    explicit LinkedListNode(const T& val) : value(val), next(nullptr) {}
};
