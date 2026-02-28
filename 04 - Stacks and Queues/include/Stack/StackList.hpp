#pragma once
#include <optional>
#include "LLNode.hpp"

// 基于单链表实现的栈。
// 链表头即栈顶。
//
// 伪代码（基于链表）：
//   Stack {
//       LinkedListNode: head
//   }
template<typename T>
struct StackList {
    LLNode<T>* head;

    StackList() : head(nullptr) {}

    // 析构函数：释放所有节点
    ~StackList() {
        while (head != nullptr) {
            LLNode<T>* next = head->next;
            delete head;
            head = next;
        }
    }

    // 禁止拷贝以避免重复释放；允许移动
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

// 入栈（Push）
template<typename T>
void Push(StackList<T>& s, const T& value);

// 出栈（Pop）—— 返回栈顶值，空栈时返回 std::nullopt
template<typename T>
std::optional<T> Pop(StackList<T>& s);

// 查看栈顶值但不移除
template<typename T>
std::optional<T> Top(const StackList<T>& s);

template<typename T>
bool IsEmpty(const StackList<T>& s);

#include "StackList.tpp"
