#pragma once
#include <optional>
#include "LLNode.hpp"

// Queue implemented as a singly linked list.
// front → head of the list (oldest element)
// back  → tail of the list (newest element)
//
// Pseudocode (linked list–based):
//   Queue {
//       LinkedListNode: front
//       LinkedListNode: back
//   }
template<typename T>
struct QueueList {
    LLNode<T>* front;
    LLNode<T>* back;

    QueueList() : front(nullptr), back(nullptr) {}

    ~QueueList() {
        while (front != nullptr) {
            LLNode<T>* next = front->next;
            delete front;
            front = next;
        }
    }

    QueueList(const QueueList&) = delete;
    QueueList& operator=(const QueueList&) = delete;
    QueueList(QueueList&& other) noexcept : front(other.front), back(other.back) {
        other.front = other.back = nullptr;
    }
    QueueList& operator=(QueueList&& other) noexcept {
        if (this != &other) {
            this->~QueueList();
            front = other.front; back = other.back;
            other.front = other.back = nullptr;
        }
        return *this;
    }
};

// Enqueue(Queue: q, Type: value)
template<typename T>
void Enqueue(QueueList<T>& q, const T& value);

// Dequeue(Queue: q) -> returns the front value, or std::nullopt if empty
template<typename T>
std::optional<T> Dequeue(QueueList<T>& q);

// Peek at the front value without removing it
template<typename T>
std::optional<T> Front(const QueueList<T>& q);

template<typename T>
bool IsEmpty(const QueueList<T>& q);

#include "QueueList.tpp"
