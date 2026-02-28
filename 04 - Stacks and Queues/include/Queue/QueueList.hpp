#pragma once
#include <optional>
#include "LLNode.hpp"

// 基于单链表实现的队列。
// front → 链表头（最早的元素）
// back  → 链表尾（最新的元素）
//
// 伪代码（基于链表）：
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

// 入队（Enqueue）
template<typename T>
void Enqueue(QueueList<T>& q, const T& value);

// 出队（Dequeue）—— 返回队首值，空队列时返回 std::nullopt
template<typename T>
std::optional<T> Dequeue(QueueList<T>& q);

// 查看队首值但不移除
template<typename T>
std::optional<T> Front(const QueueList<T>& q);

template<typename T>
bool IsEmpty(const QueueList<T>& q);

#include "QueueList.tpp"
