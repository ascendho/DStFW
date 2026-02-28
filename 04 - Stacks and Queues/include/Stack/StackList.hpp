#pragma once
#include <optional>
#include "LLNode.hpp"

// Stack implemented as a singly linked list.
// The head of the list is the top of the stack.
//
// Pseudocode (linked list–based):
//   Stack {
//       LinkedListNode: head
//   }
template<typename T>
struct StackList {
    LLNode<T>* head;

    StackList() : head(nullptr) {}

    // Destructor: free all nodes
    ~StackList() {
        while (head != nullptr) {
            LLNode<T>* next = head->next;
            delete head;
            head = next;
        }
    }

    // Non-copyable to avoid double-free; moveable
    StackList(const StackList&) = delete;
    StackList& operator=(const StackList&) = delete;
    StackList(StackList&& other) noexcept : head(other.head) { other.head = nullptr; }
    StackList& operator=(StackList&& other) noexcept {
        if (this != &other) {
            this->~StackList();
            head = other.head;
            other.head = nullptr;
        }
        return *this;
    }
};

// Push(Stack: s, Type: value)
template<typename T>
void Push(StackList<T>& s, const T& value);

// Pop(Stack: s) -> returns the top value, or std::nullopt if empty
template<typename T>
std::optional<T> Pop(StackList<T>& s);

// Peek at the top value without removing it
template<typename T>
std::optional<T> Top(const StackList<T>& s);

template<typename T>
bool IsEmpty(const StackList<T>& s);

#include "StackList.tpp"
