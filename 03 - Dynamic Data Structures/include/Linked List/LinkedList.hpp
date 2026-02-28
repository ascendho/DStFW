#pragma once
#include <stdexcept>
#include "LinkedListNode.hpp"

// LinkedListLookUp - Chapter 3: Dynamic Data Structures
// Returns a pointer to the node at the given element_number (0-based index).
// Returns nullptr if the index is out of range.
template<typename T>
LinkedListNode<T>* LinkedListLookUp(LinkedListNode<T>* head, int element_number);

// LinkedListInsertAfter - Chapter 3: Dynamic Data Structures
// Inserts new_node immediately after previous in the list.
template<typename T>
void LinkedListInsertAfter(LinkedListNode<T>* previous, LinkedListNode<T>* new_node);

// LinkedListInsert - Chapter 3: Dynamic Data Structures
// Inserts a new node with the given value at position index (0-based).
// Returns the (possibly new) head of the list.
// Throws std::out_of_range if index is more than one past the end.
template<typename T>
LinkedListNode<T>* LinkedListInsert(LinkedListNode<T>* head, int index, const T& value);

// LinkedListDelete - Chapter 3: Dynamic Data Structures
// Deletes the node at position index (0-based).
// Returns the (possibly new) head of the list, or nullptr if the list is empty.
// Throws std::out_of_range if index is out of range.
template<typename T>
LinkedListNode<T>* LinkedListDelete(LinkedListNode<T>* head, int index);

#include "LinkedList.tpp"
