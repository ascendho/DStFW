#pragma once

// 用于栈/队列链表实现的链表节点
// （第四章独立使用的本地副本）
template<typename T>
struct LLNode {
    T value;
    LLNode<T>* next;

    explicit LLNode(const T& val) : value(val), next(nullptr) {}
};
